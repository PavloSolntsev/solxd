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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionIndex_Files,SIGNAL(activated()),this,SLOT(indexDatabase()));
    connect(ui->actionSettings,SIGNAL(activated()),this,SLOT(runSettings()));
    connect(ui->actionStart,SIGNAL(activated()),this,SLOT(startSearch()));
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(crystinfowindow(QListWidgetItem*)));
    sform = NULL;
    dia = new Settings(this);
    DBpath = QDir(dia->dbpath()).filePath("solxd.database");
// Read information about Toolbar icon size
    ui->mainToolBar->setIconSize(QSize(dia->getToolbarSize(),dia->getToolbarSize()));
    connect(dia,SIGNAL(toolbarIconsChanged(int)),this,SLOT(setToolbarIcons(int)));

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
    bool includeshelxle = dia->excludeshelxlefolders();

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

            if(includeshelxle && dirIt.filePath().contains("shelXlesaves",Qt::CaseInsensitive))
                continue;

            if (dirIt.fileInfo().isFile())
              {
//                qDebug() << dirIt.next();
                QString suffix = dirIt.fileInfo().suffix().toLower();

                if (suffix == "ins" || suffix == "res" || suffix == "cif")
                {
                    QString filepath = dirIt.filePath();
//                    database.push_back(Crystfile(filetypemap[suffix],filepath));
                    ui->statusBar->showMessage(tr("Indexing file: %1").arg(filepath));
                    Crystfile crf(filetypemap[suffix],filepath);
                    out << crf;
                    QVariant qv;
                    qv.setValue(crf);
                    QListWidgetItem *listiteam = new QListWidgetItem();
                    listiteam->setText(filepath);
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
//        ui->listWidget->addItem(it->getPath());
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

//        int ret = QMessageBox::information(this, tr("Information about file"),
//                                        cellinfo,
//                                        QMessageBox::Ok);
        QMessageBox msgbox;
        msgbox.setWindowTitle(tr("Information about file"));
        msgbox.setText(QFileInfo(crfile.getPath()).fileName());
        msgbox.setInformativeText(cellinfo);
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.setDefaultButton(QMessageBox::Ok);
        msgbox.setDetailedText(tr("Put some text here"));
//        msgbox.setGeometry(msgbox.pos().x(),msgbox.pos().y(),500,500);
        msgbox.exec();

}
