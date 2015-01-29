#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDebug>

#include <iostream>

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
    int row;

    for(QStringList::Iterator it=searchfolders.begin();it!=searchfolders.end();it++)
    {
        QDirIterator dirIt(*it,QDirIterator::Subdirectories);
        while (dirIt.hasNext()) {
            dirIt.next();
            if (QFileInfo(dirIt.filePath()).isFile())
            {
                if (QFileInfo(dirIt.filePath()).suffix() == "res")
                {
                    searchresults.append(dirIt.filePath());
                    ui->listWidget->insertItem(row++,dirIt.filePath());
                }
                if (QFileInfo(dirIt.filePath()).suffix() == "ins")
                {
                    searchresults.append(dirIt.filePath());
                    ui->listWidget->insertItem(row++,dirIt.filePath());
                }
                if (QFileInfo(dirIt.filePath()).suffix() == "cif")
                {
                    searchresults.append(dirIt.filePath());
                    ui->listWidget->insertItem(row++,dirIt.filePath());
                }


            }
        }
    }
}

void MainWindow::runSettings()
{
    dia = new Settings();
    dia->show();
}
