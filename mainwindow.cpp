#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDebug>
#include <iostream>
#include "crystfile.h"
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionIndex_Files,SIGNAL(activated()),this,SLOT(indexDatabase()));
    connect(ui->actionSettings,SIGNAL(activated()),this,SLOT(runSettings()));
    dia = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dia;
}


void MainWindow::indexDatabase()
{
// Read database folder path

    Settings *set = new Settings(this);
    QString DBpath = set->dbpath();
    QStringList searchfolders = set->indexpath();

    QStringList searchresults;

    delete set;
//    int row;

    QMap<QString,FileType> filetypemap;

    filetypemap["ins"] = INS;
    filetypemap["res"] = RES;
    filetypemap["cis"] = CIF;

    QList<Crystfile> database;

    ui->listWidget->clear();

    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(openfile(QListWidgetItem*)));

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
                    database.push_back(Crystfile(filetypemap[suffix],dirIt.filePath()));
//                    searchresults.append(dirIt.filePath());
//                    ui->listWidget->insertItem(row++,dirIt.filePath());
                    ui->listWidget->addItem(dirIt.filePath());
                }
            }
        } // end while
    } // end for loop


}

void MainWindow::runSettings()
{
    dia = new Settings();
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
