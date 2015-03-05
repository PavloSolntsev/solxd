#include "settings.h"
#include "ui_settings.h"
#include <QFileDialog>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    connect(ui->pushButtonAdd1,SIGNAL(clicked()),this,SLOT(addPath1()));
    connect(ui->pushButtonAdd2,SIGNAL(clicked()),this,SLOT(addPath2()));
    connect(ui->pushButtonAdd3,SIGNAL(clicked()),this,SLOT(addPath3()));
    connect(ui->pushButtonDBAdd,SIGNAL(clicked()),this,SLOT(addDBPath()));

    connect(ui->pushButtonDelete1,SIGNAL(clicked()),this,SLOT(deletePath1()));
    connect(ui->pushButtonDelete2,SIGNAL(clicked()),this,SLOT(deletePath2()));
    connect(ui->pushButtonDelete3,SIGNAL(clicked()),this,SLOT(deletePath3()));
    connect(ui->pushButtonDBDelete,SIGNAL(clicked()),this,SLOT(deleteDBPath()));

    connect(ui->lineeditPath1,SIGNAL(textChanged(QString)),this,SLOT(path1manualchange(QString)));
    connect(ui->lineeditPath2,SIGNAL(textChanged(QString)),this,SLOT(path2manualchange(QString)));
    connect(ui->lineeditPath3,SIGNAL(textChanged(QString)),this,SLOT(path3manualchange(QString)));

    connect(ui->pushButtonOk,SIGNAL(clicked()),this,SLOT(ok_button_clicked()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->spinBox_iconsize,SIGNAL(valueChanged(int)),parent,SLOT(setToolbarIcons(int)));

    settings = new QSettings( QSettings::IniFormat, QSettings::UserScope ,PROGRAM_NAME,PROGRAM_NAME ,this);


    if(settings->contains("IndexPath1"))
    {
        path1 = settings->value("IndexPath1").toString();
        ui->lineeditPath1->setText(path1);
        listpath.append(path1);
    }

    if(settings->contains("IndexPath2"))
    {
        path2 = settings->value("IndexPath2").toString();
        ui->lineeditPath2->setText(path2);
        listpath.append(path2);
    }

    if(settings->contains("IndexPath3"))
    {
        path3 = settings->value("IndexPath3").toString();
        ui->lineeditPath3->setText(path3);
        listpath.append(path3);
    }

    if(settings->contains("PathDB"))
    {
        pathDB = settings->value("PathDB").toString();
        ui->lineEditDB->setText(pathDB);
    }
    else
        ui->lineEditDB->setText(QDir::homePath());

    if (settings->contains("ToolBarSize")) {
        toolbarsize = settings->value("ToolBarSize").toInt();
        ui->spinBox_iconsize->setValue(toolbarsize);
        emit toolbarIconsChanged(toolbarsize);
    }

}

Settings::~Settings()
{

    delete settings;
    delete ui;

}

void Settings::ok_button_clicked()
{
    listpath.clear();
    if(!path1.isEmpty())
    {
        settings->setValue("IndexPath1", path1);
        listpath.append(path1);
    }
    if(!path2.isEmpty())
    {
        settings->setValue("IndexPath2", path2);
        listpath.append(path2);
    }
    if(!path3.isEmpty())
    {
        settings->setValue("IndexPath3", path3);
        listpath.append(path3);
    }
    if(!pathDB.isEmpty())
        settings->setValue("PathDB", pathDB);

    int tbsize(ui->spinBox_iconsize->value());

    if (tbsize != toolbarsize)
        settings->setValue("ToolBarSize",tbsize);


    settings->sync();
    close();
}

void Settings::addPath1()
{

    path1 = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->lineeditPath1->setText(path1);
}

void Settings::addPath2()
{


    path2 = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineeditPath2->setText(path2);
}

void Settings::addPath3()
{

    path3 = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineeditPath3->setText(path3);
}

void Settings::addDBPath()
{
    pathDB = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineEditDB->setText(pathDB);

}

void Settings::deletePath1()
{
    ui->lineeditPath1->clear();
    path1.clear();
    settings->remove("IndexPath1");

}

void Settings::deletePath2()
{
    ui->lineeditPath2->clear();
    path2.clear();
    settings->remove("IndexPath2");
}

void Settings::deletePath3()
{
    ui->lineeditPath3->clear();
    path3.clear();
    settings->remove("IndexPath3");
}

void Settings::deleteDBPath()
{
    ui->lineEditDB->clear();
    pathDB.clear();
    settings->remove("PathDB");
}

