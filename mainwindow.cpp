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
#include <QDockWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionIndex_Files,SIGNAL(activated()),this,SLOT(indexDatabase()));
    connect(ui->actionSettings,SIGNAL(activated()),this,SLOT(runSettings()));
//    connect(ui->actionStart,SIGNAL(activated()),this,SLOT(startSearch()));
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openfile(QListWidgetItem*)));
    connect(ui->actionAbout_Qt,SIGNAL(activated()),this,SLOT(aboutQt()));
    connect(ui->actionSolXd_help,SIGNAL(activated()),this,SLOT(help()));
    connect(ui->action_About,SIGNAL(activated()),this,SLOT(aboutdialog()));

//    sform = NULL;
    dia = new Settings(this);
    DBpath = QDir(dia->dbpath()).filePath("solxd.database");
// Read information about Toolbar icon size
    ui->mainToolBar->setIconSize(QSize(dia->getToolbarSize(),dia->getToolbarSize()));
    ui->listWidget->setFont(dia->get_lwfont());
    qDebug() << "File " << __FILE__ << " Line " << __LINE__;
    connect(dia,SIGNAL(toolbarIconsChanged(int)),this,SLOT(setToolbarIcons(int)));
    connect(dia,SIGNAL(fontChanged(QFont)),this,SLOT(changelwfont(QFont)));

    dock = new QDockWidget(tr("Search window"),this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    sform = new SearchForm(dock);
    sform->setDBfile(DBpath);
    dock->setWidget(sform);
    dock->setObjectName(tr("Search window"));
    ui->menu_View->addAction(dock->toggleViewAction());
    addDockWidget(Qt::RightDockWidgetArea,dock);

//    searchw = new QAction(tr("search window"),this);
//    searchw->setCheckable(true);

//    ui->menu_View->addAction(searchw);
//    dock->setVisible(false);
//    connect(searchw,SIGNAL(toggled(bool)),dock,SLOT(setVisible(bool)));
    connect(sform,SIGNAL(finished(const QList<Crystfile>&)),this,SLOT(outputResults(const QList<Crystfile>&)));
    connect(sform,SIGNAL(massage(QString)),this,SLOT(displaymassage(QString)));

    about = new About(this);


    checktime();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete dia;
    delete sform;
    delete about;
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
//            qDebug() << "File: "<< __FILE__ << " Line: " << __LINE__  ;
//            qDebug() << dirIt.filePath();
            dirIt.next();
//            qDebug() << "File: "<< __FILE__ << " Line: " << __LINE__  ;
            if(excludeshelxle && dirIt.filePath().contains("shelXlesaves",Qt::CaseInsensitive))
                continue;
//            qDebug() << "File: "<< __FILE__ << " Line: " << __LINE__  ;
            if(excludeolex2 && dirIt.filePath().contains(".olex",Qt::CaseInsensitive))
                continue;
//            qDebug() << "File: "<< __FILE__ << " Line: " << __LINE__  ;
            if (dirIt.fileInfo().isFile()){
                QString suffix = dirIt.fileInfo().suffix().toLower();
//            qDebug() << "File: "<< __FILE__ << " Line: " << __LINE__  ;
                if (suffix == "ins" || suffix == "res" || suffix == "cif")
                {
                    QString filepath = dirIt.filePath();
                    ui->statusBar->showMessage(tr("Indexing file: %1").arg(filepath));
                    Crystfile crf(filetypemap[suffix],filepath);
//                    if (crf.error().size() == 0)
//                        crf.niggli();

                    out << crf;
                    QVariant qv;
                    qv.setValue(crf);
                    QListWidgetItem *listiteam = new QListWidgetItem();
                    listiteam->setText(filepath);

                    if (crf.error().contains(Crystfile::CRFORMULAERROR))
                        listiteam->setForeground(Qt::red);
                    else {
                        if (crf.error().contains(Crystfile::CRINSERROR))
                            listiteam->setForeground(Qt::blue);
                        else {
                            if (crf.error().contains(Crystfile::CRLATTERROR))
                                listiteam->setForeground(Qt::magenta);
                            else{
                                if (crf.error().contains(Crystfile::CRCELLERORR))
                                    listiteam->setForeground(Qt::green);
                            }
                        }
                    }

                    //                    qDebug() << "0Unit a =" << crf.a();


                    listiteam->setData(Qt::UserRole,qv);
                    ui->listWidget->addItem(listiteam);
//                    qDebug() << "Finished work on " << filepath;
                }
            }
//            qDebug() << "File: "<< __FILE__ << " Line: " << __LINE__  ;
        } // end while
    } // end for loop
    DBFile.close();
    ui->statusBar->showMessage(tr("Indexing finished"));
//    qDebug() << "File: "<< __FILE__ << " Line: " << __LINE__  ;
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
//        it->niggli();
        ui->listWidget->addItem(listiteam);

        if (it->error().size() != 0)
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
//    QStringList arguments;
//    for(int i=0; i<ui->listWidget->selectedItems().size();i++){
//        QString path = QDir::toNativeSeparators(ui->listWidget->selectedItems().at(i)->text());
//        QDesktopServices::openUrl(QUrl("file:///" + path));
//    }

    QString program = dia->getTextEditor();
    QStringList arguments;


    if (program.isEmpty()) {
        for(int i=0; i<ui->listWidget->selectedItems().size();i++){
                QString path = QDir::toNativeSeparators(ui->listWidget->selectedItems().at(i)->text());
                QDesktopServices::openUrl(QUrl("file:///" + path));
        }
    } else {
        for(int i=0; i<ui->listWidget->selectedItems().size();i++){
                arguments << ui->listWidget->selectedItems().at(i)->text();
        }

        QProcess *myProcess = new QProcess(this);
        myProcess->start(program, arguments);
    }
}

void MainWindow::deleteselectedfiles()
{
    QStringList arguments;

    for(int i=0; i<ui->listWidget->selectedItems().size();i++)
        arguments << ui->listWidget->selectedItems().at(i)->text();

    QMessageBox::StandardButton i = QMessageBox::warning(this,"Delete Files","Do you really wnat to delete these files:",QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,QMessageBox::No);

    switch (i) {
    case QMessageBox::Yes:
    {
        QString badfiles;
        QString goodfiles;
        for (int var = 0; var < arguments.size(); ++var) {
            if(!QFile::remove(arguments.at(var)))
                badfiles.append(arguments.at(var)) +"\n";
            else
                goodfiles.append(arguments.at(var)) +"\n";
        }

        if(!badfiles.isEmpty())
            QMessageBox::information(this,"Can't delete file(s)",badfiles,QMessageBox::Close);

        if(!goodfiles.isEmpty())
            QMessageBox::information(this,"Successfully delteted file(s)",goodfiles,QMessageBox::Close);
    }
        break;
    case QMessageBox::No:
    case QMessageBox::Cancel:
        break;
    default:
        break;
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

void MainWindow::aboutQt()
{
//    QApplication::aboutQt();
    QMessageBox::aboutQt ( this, tr("SolXd about Qt dialog"));

}

void MainWindow::aboutdialog()
{

    about->show();
}

void MainWindow::help()
{
    helpdialog=new QTextEdit(this);
    helpdialog->setWindowFlags(Qt::Window); //or Qt::Tool, Qt::Dialog if you like

//    QDir progdir(QDir::currentPath());

//    QString manual(QDir(QDir::currentPath()).filePath("MANUAL"));

    helpdialog->setReadOnly(true);
    helpdialog->append("<h2>Help</h2>");

    QFile manual(":/doc/MANUAL");

    if (QFileInfo(manual).exists()) {
        if(!manual.open(QIODevice::ReadOnly | QIODevice::Text)){
            helpdialog->append("Manual file can't be opened");
        }
        else{
            QTextStream stream(&manual);
            helpdialog->append(stream.readAll());
        }
    }
    else
        helpdialog->append("Manual file can't be found");

    helpdialog->show();
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

    deletefiles = new QAction(tr("&Delete file(s)"),contextmenu);
    deletefiles->setStatusTip(tr("Delete file permanently from the disk"));
    connect(deletefiles,SIGNAL(triggered()),this,SLOT(deleteselectedfiles()));

    contextmenu->addAction(aopenfiles);
    contextmenu->addAction(openastext);
    contextmenu->addAction(browsfiles);
    contextmenu->addAction(deletefiles);
    contextmenu->exec(event->globalPos());
}
