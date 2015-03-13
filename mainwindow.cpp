/**************************************************************************
**
** This mainwindow.cpp file is part of SolXd software.
**
** Copyright (C) 2015 Pavlo Solntsev
**
** SolXd is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Foobar is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDebug>
#include <iostream>
#include "crystfile.h"
#include <QProcess>
#include <QFile>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QLabel>
//#include <QCursor>
#include <QMessageBox>
#include <QDateTime>
#include "solXd.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionIndex_Files,SIGNAL(activated()),this,SLOT(indexDatabase()));
    connect(ui->actionSettings,SIGNAL(activated()),this,SLOT(runSettings()));
    connect(ui->actionStart,SIGNAL(activated()),this,SLOT(startSearch()));
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openfile(QListWidgetItem*)));    
    sform = NULL;
    dia = new Settings(this);
    DBpath = QDir(dia->dbpath()).filePath("solxd.database");
// Read information about Toolbar icon size
    ui->mainToolBar->setIconSize(QSize(dia->getToolbarSize(),dia->getToolbarSize()));
    ui->listWidget->setFont(dia->get_lwfont());
    qDebug() << "File " << __FILE__ << " Line " << __LINE__;
    connect(dia,SIGNAL(toolbarIconsChanged(int)),this,SLOT(setToolbarIcons(int)));
    connect(dia,SIGNAL(fontChanged(QFont)),this,SLOT(changelwfont(QFont)));

    checktime();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dia;
    delete sform;
}


void MainWindow::indexDatabase()
{
// Read database folder path
    ui->statusBar->showMessage(tr("Indexing strated..."));

    DBpath = QDir(dia->dbpath()).filePath("solxd.database");
    bool excludeshelxle = dia->excludeshelxlefolders();
    bool excludeolex2 = dia->excludeolex2folders();

    if (QFile(DBpath).exists()) {
        try {
            if (!QFile::remove(DBpath))
                throw "Can't delete database file";
            else
                qDebug() << "File: " << DBpath << " was succefully deleted";
        } catch (const char *a) {
            qDebug() << a;
        }
    }

    QStringList searchfolders = dia->indexpath();

    QMap<QString,FileType> filetypemap;

    filetypemap["ins"] = INS;
    filetypemap["res"] = RES;
    filetypemap["cif"] = CIF;

    ui->listWidget->clear();

    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openfile(QListWidgetItem*)));

    QFile DBFile(DBpath);
    DBFile.open(QIODevice::WriteOnly);
    QDataStream out(&DBFile);

    out << quint32(0x12345678);
    out << quint16(out.version());
    out << QDateTime::currentDateTime();

    out.setVersion(out.version());
/**
 * 123 corresponds QDataStream::Qt_4_8
 *
 */
    for(QStringList::Iterator it=searchfolders.begin();it!=searchfolders.end();it++)
    {
        QDirIterator dirIt(*it,QDirIterator::Subdirectories);
        while (dirIt.hasNext()) {
            dirIt.next();

            if(excludeshelxle && dirIt.filePath().contains("shelXlesaves",Qt::CaseInsensitive))
                continue;

            if(excludeolex2 && dirIt.filePath().contains(".olex",Qt::CaseInsensitive))
                continue;

            if (dirIt.fileInfo().isFile())
              {
                QString suffix = dirIt.fileInfo().suffix().toLower();

                if (suffix == "ins" || suffix == "res" || suffix == "cif")
                {
                    QString filepath = dirIt.filePath();
                    ui->statusBar->showMessage(tr("Indexing file: %1").arg(filepath));
                    Crystfile crf(filetypemap[suffix],filepath);
                    out << crf;
                    QVariant qv;
                    qv.setValue(crf);
                    QListWidgetItem *listiteam = new QListWidgetItem();
                    listiteam->setText(filepath);
                    if (!crf.state())
                        listiteam->setForeground(Qt::red);

                    if (crf.cifblock() > 1)
                        listiteam->setForeground(Qt::blue);


                    listiteam->setData(Qt::UserRole,qv);
                    ui->listWidget->addItem(listiteam);
                }
            }
        } // end while
    } // end for loop
    DBFile.close();
    ui->statusBar->showMessage(tr("Indexing finished"));
}

void MainWindow::runSettings()
{
    dia->setWindowFlags(Qt::Window);
    dia->show();
}

void MainWindow::openfile(QListWidgetItem *iteam)
{
    qDebug() << "open file function is working ok";
    qDebug() << iteam->text();

    QString program = dia->getViewer();
    QStringList arguments;
    arguments << iteam->text();

    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
}

void MainWindow::startSearch()
{
    if (sform != NULL) {
        sform->show();
    } else {
        sform = new SearchForm(this);
        connect(sform,SIGNAL(finished(const QList<Crystfile>&)),this,SLOT(outputResults(const QList<Crystfile>&)));
        connect(sform,SIGNAL(massage(QString)),this,SLOT(displaymassage(QString)));
        sform->setWindowFlags(Qt::Window);
        sform->setDBfile(DBpath);
        sform->show();
    }
}

void MainWindow::outputResults(const QList<Crystfile> &res)
{
    ui->listWidget->clear();
    for (QList<Crystfile>::const_iterator it = res.begin(); it != res.end(); ++it) {
        QVariant qv;
        qv.setValue(*it);
        QListWidgetItem *listiteam = new QListWidgetItem();
        listiteam->setText(it->getPath());
        listiteam->setData(Qt::UserRole,qv);
        ui->listWidget->addItem(listiteam);

        if (it->state() == Crystfile::CRGOOD)
            listiteam->setForeground(Qt::red);

        if (it->cifblock() > 1 && it->getFileType() == CIF)
            listiteam->setForeground(Qt::blue);

    }
    ui->statusBar->showMessage(tr("%1 files have been found").arg(res.size()));
}

void MainWindow::setToolbarIcons(const int &i)
{
    ui->mainToolBar->setIconSize(QSize(i,i));
    ui->mainToolBar->update();
}

void MainWindow::crystinfowindow(QListWidgetItem *item)
{
        QVariant *qvr = new QVariant(item->data(Qt::UserRole));
        Crystfile crfile(qvr->value<Crystfile>());
        QString cellinfo;
        cellinfo = QString("<b>Unit Cell:</b>\n%1\t%2\t%3\t%4\t%5\t%6>").arg(crfile.a()).arg(crfile.b()).arg(crfile.c()).arg(crfile.alpha()).arg(crfile.beta()).arg(crfile.gama());

        QMessageBox msgbox;
        msgbox.setWindowTitle(tr("Information about file"));
        msgbox.setText(QFileInfo(crfile.getPath()).fileName());
        msgbox.setInformativeText(cellinfo);
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.setDefaultButton(QMessageBox::Ok);
        msgbox.setDetailedText(tr("Put some text here"));
        msgbox.exec();

}

void MainWindow::checktime()
{
    QSettings tset(PROGRAM_NAME,PROGRAM_NAME);
    QString dbfile = QDir(dia->dbpath()).filePath("solxd.database");
    QDateTime dtfromfile;

    if (QFileInfo(dbfile).isFile()) {
        QFile DBFile(dbfile);
        DBFile.open(QIODevice::ReadOnly);
        QDataStream in(&DBFile);

        quint32 magic;
        quint16 version;

        in >> magic >> version;

        try {
            if (magic != quint32(0x12345678))
                throw "Wrong file format. Database file has been broken. Reindex the database.";
            else
            {
                if (version > in.version())
                    throw "File is from a more recent version of the application";
                else
                    qDebug() << "Version was passed sussecfully";
            }
        } catch (const char *s) {
            qDebug() << s;
        }

        in.setVersion(version);
        in >> dtfromfile;

        QMap<int,QString> monthmap;
        monthmap[1] = "Jan";
        monthmap[2] = "Feb";
        monthmap[3] = "Mar";
        monthmap[4] = "Apr";
        monthmap[5] = "May";
        monthmap[6] = "Jun";
        monthmap[7] = "Jul";
        monthmap[8] = "Aug";
        monthmap[9] = "Sep";
        monthmap[10] = "Oct";
        monthmap[11] = "Nov";
        monthmap[12] = "Dec";

        int year(dtfromfile.date().year());
        int day(dtfromfile.date().day());
        int hour(dtfromfile.time().hour());
        int minute(dtfromfile.time().minute());

        QString text("Database file was created ");
        text += monthmap[dtfromfile.date().month()];
        text.append(QString("%1, %2, %3:%4").arg(day).arg(year).arg(hour).arg(minute));

        ui->statusBar->showMessage(text);
        DBFile.close();
    }
}

void MainWindow::openlistwidgetfiles()
{
    QString program = dia->getViewer();
    QStringList arguments;

    for(int i=0; i<ui->listWidget->selectedItems().size();i++)
        arguments << ui->listWidget->selectedItems().at(i)->text();

    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
}

void MainWindow::openbrowsfiles()
{
    QStringList arguments;

    for(int i=0; i<ui->listWidget->selectedItems().size();i++){
        QString path = QDir::toNativeSeparators(ui->listWidget->selectedItems().at(i)->text());
        QDesktopServices::openUrl(QUrl("file:///" + QFileInfo(path).dir().absolutePath()));
    }
}

void MainWindow::openfilesastext()
{
        QStringList arguments;

        for(int i=0; i<ui->listWidget->selectedItems().size();i++){
            QString path = QDir::toNativeSeparators(ui->listWidget->selectedItems().at(i)->text());
            QDesktopServices::openUrl(QUrl("file:///" + path));
        }
}

void MainWindow::changelwfont(const QFont &font)
{
    ui->listWidget->setFont(font);
}

void MainWindow::displaymassage(const QString &text)
{
    ui->statusBar->showMessage(text);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    contextmenu = new QMenu(this);

    aopenfiles = new QAction(tr("&Open with viewer"),contextmenu);
    aopenfiles->setStatusTip(tr("Open selected file(s) with viewer"));
    connect(aopenfiles,SIGNAL(triggered()),this,SLOT(openlistwidgetfiles()));

    browsfiles = new QAction(tr("&Locate files"),contextmenu);
    browsfiles->setStatusTip(tr("Open files in default file browser"));
    connect(browsfiles,SIGNAL(triggered()),this,SLOT(openbrowsfiles()));

    openastext = new QAction(tr("&Open file(s) in editor"),contextmenu);
    openastext->setStatusTip(tr("Open files in default text editor"));
    connect(openastext,SIGNAL(triggered()),this,SLOT(openfilesastext()));

    contextmenu->addAction(aopenfiles);
    contextmenu->addAction(openastext);
    contextmenu->addAction(browsfiles);
    contextmenu->exec(event->globalPos());
}
