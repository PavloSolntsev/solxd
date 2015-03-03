#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDebug>
#include <iostream>
#include "crystfile.h"
#include <QProcess>
#include <QFile>

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
    QStringList searchfolders = set->indexpath();
    delete set;

    qDebug() << "DB file is " << DBpath;
 //   QStringList searchresults;

    QMap<QString,FileType> filetypemap;

    filetypemap["ins"] = INS;
    filetypemap["res"] = RES;
    filetypemap["cis"] = CIF;

//    QList<Crystfile> database;

    ui->listWidget->clear();

    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(openfile(QListWidgetItem*)));

    QFile DBFile(DBpath);
    DBFile.open(QIODevice::WriteOnly);
    QDataStream out(&DBFile);

    out << (quint32)0xA0B0C0D0;
    out << (qint32)123;
    out.setVersion(QDataStream::Qt_4_6);
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
    sform = new SearchForm(this);
    sform->setWindowFlags(Qt::Window);
    sform->setDBfile(DBpath);
    sform->show();
}
