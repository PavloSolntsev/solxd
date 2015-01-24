#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    std::cout << "Hello World!" << std::endl;

}

void MainWindow::runSettings()
{
    dia = new Settings();
    dia->show();
}
