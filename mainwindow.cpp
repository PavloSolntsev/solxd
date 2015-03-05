#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDebug>
#include <iostream>
#include "crystfile.h"
#include <QProcess>
#include <QFile>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionIndex_Files,SIGNAL(activated()),this,SLOT(indexDatabase()));
    connect(ui->actionSettings,SIGNAL(activated()),this,SLOT(runSettings()));
    connect(ui->actionStart,SIGNAL(activated()),this,SLOT(startSearch()));
    dia = NULL;
    sform = NULL;
    Settings *set = new Settings(this);
    DBpath = QDir(set->dbpath()).filePath("solxd.database");
// Read information about Toolbar icon size
    ui->mainToolBar->setIconSize(QSize(set->getToolbarSize(),set->getToolbarSize()));
    connect(set,SIGNAL(toolbarIconsChanged(int)),this,SLOT(setToolbarIcons(int)));
    delete set;

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

    Settings *set = new Settings(this);
    DBpath = QDir(set->dbpath()).filePath("solxd.database");

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

    QStringList searchfolders = set->indexpath();
    delete set;

//    qDebug() << "DB file is " << DBpath;
 //   QStringList searchresults;

    QMap<QString,FileType> filetypemap;

    filetypemap["ins"] = INS;
    filetypemap["res"] = RES;
    filetypemap["cif"] = CIF;

//    QList<Crystfile> database;

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

            if (dirIt.fileInfo().isFile())
              {
//                qDebug() << dirIt.next();
                QString suffix = dirIt.fileInfo().suffix().toLower();

                if (suffix == "ins" || suffix == "res" || suffix == "cif")
                {
                    QString filepath = dirIt.filePath();
//                    database.push_back(Crystfile(filetypemap[suffix],filepath));
                    out << Crystfile(filetypemap[suffix],filepath);
                    ui->statusBar->showMessage(filepath);
                    ui->listWidget->addItem(filepath);
                }
            }
        } // end while
    } // end for loop
    DBFile.close();
    ui->statusBar->showMessage(tr("Indexing finished"));
}

void MainWindow::runSettings()
{
    dia = new Settings(this);
    dia->setWindowFlags(Qt::Window);
    dia->show();
}

void MainWindow::openfile(QListWidgetItem *iteam)
{
    qDebug() << "open file function is working ok";
    qDebug() << iteam->text();

    QString program = "/home/pavlo/bin/mercury";
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
        ui->listWidget->addItem(it->getPath());
    }
    ui->statusBar->showMessage(tr("%1 files have been found").arg(res.size()));

}

void MainWindow::setToolbarIcons(const int &i)
{
    ui->mainToolBar->setIconSize(QSize(i,i));
    ui->mainToolBar->update();
}

