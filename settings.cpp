/**************************************************************************
**
** This settings.cpp file is part of SolXd software.
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

#include "settings.h"
#include "ui_settings.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    hide();

    connect(ui->pushButtonAdd1,SIGNAL(clicked()),this,SLOT(addPath1()));
    connect(ui->pushButtonAdd2,SIGNAL(clicked()),this,SLOT(addPath2()));
    connect(ui->pushButtonAdd3,SIGNAL(clicked()),this,SLOT(addPath3()));
    connect(ui->pushButtonDBAdd,SIGNAL(clicked()),this,SLOT(addDBPath()));
    connect(ui->pushButton_addviewwr,SIGNAL(clicked()),this,SLOT(addviewer()));
    connect(ui->pushButton_addtexteditor,SIGNAL(clicked()),this,SLOT(addtexteditor()));

    connect(ui->pushButtonDelete1,SIGNAL(clicked()),this,SLOT(deletePath1()));
    connect(ui->pushButtonDelete2,SIGNAL(clicked()),this,SLOT(deletePath2()));
    connect(ui->pushButtonDelete3,SIGNAL(clicked()),this,SLOT(deletePath3()));
    connect(ui->pushButtonDBDelete,SIGNAL(clicked()),this,SLOT(deleteDBPath()));
    connect(ui->pushButton_deleteviewer,SIGNAL(clicked()),this,SLOT(deleteviewer()));
    connect(ui->pushButton_deletetexteditor,SIGNAL(clicked()),this,SLOT(deletetexteditor()));

    connect(ui->lineeditPath1,SIGNAL(textChanged(QString)),this,SLOT(path1manualchange(QString)));
    connect(ui->lineeditPath2,SIGNAL(textChanged(QString)),this,SLOT(path2manualchange(QString)));
    connect(ui->lineeditPath3,SIGNAL(textChanged(QString)),this,SLOT(path3manualchange(QString)));
    connect(ui->lineEdit_viewer,SIGNAL(textChanged(QString)),this,SLOT(viewerpathchanged(QString)));
    connect(ui->lineEdit_texteditor,SIGNAL(textChanged(QString)),this,SLOT(texteditorchanged(QString)));

    connect(ui->pushButtonOk,SIGNAL(clicked()),this,SLOT(ok_button_clicked()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->spinBox_iconsize,SIGNAL(valueChanged(int)),parent,SLOT(setToolbarIcons(int)));
    connect(ui->pushButton_font,SIGNAL(clicked()),this,SLOT(font_button_clicked()));

    connect(ui->checkBox_shelxle,SIGNAL(stateChanged(int)),this,SLOT(shelxlecheckchanged(int)));
    connect(ui->checkBox_olex2,SIGNAL(stateChanged(int)),this,SLOT(olex2checkchanged(int)));

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

    if (settings->contains("ShelXle")) {
        shelxlecheck = settings->value("ShelXle").toBool();
        ui->checkBox_shelxle->setChecked(shelxlecheck);
    }

    if (settings->contains("Olex2")) {
        olex2check = settings->value("Olex2").toBool();
        ui->checkBox_olex2->setChecked(olex2check);
    }

    if(settings->contains("PathDB"))
    {
        pathDB = settings->value("PathDB").toString();
        ui->lineEditDB->setText(pathDB);
    }
    else
        ui->lineEditDB->setText(QDir::homePath());

    if (settings->contains("ViewerPath")) {
        viewerpath = settings->value("ViewerPath").toString();
        ui->lineEdit_viewer->setText(viewerpath);
    }

    if (settings->contains("TextEditorPath")) {
        texteditorpath = settings->value("TextEditorPath").toString();
        ui->lineEdit_texteditor->setText(texteditorpath);
    }

    if (settings->contains("ToolBarSize")) {
        toolbarsize = settings->value("ToolBarSize").toInt();
        ui->spinBox_iconsize->setValue(toolbarsize);
        emit toolbarIconsChanged(toolbarsize);
    }

    if (settings->contains("item.font.family")) {
        lwfont.setFamily(settings->value("item.font.family").toString());
        lwfont.setPointSize(settings->value("item.font.size").toInt());
 //       qDebug() << "File = " << __FILE__ << " Line " << __LINE__ << "Point size = " << settings->value("item.font.size").toInt();
        lwfont.setBold(settings->value("item.font.bold").toBool());
        lwfont.setItalic(settings->value("item.font.italic").toBool());
    }

    ui->pushButton_font->setText(lwfont.family()+", "+QString::number(lwfont.pointSize()));
    emit fontChanged(lwfont);
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

    if (!viewerpath.isEmpty()) {
        settings->setValue("ViewerPath",viewerpath);
    }

    if (!texteditorpath.isEmpty()) {
        settings->setValue("TextEditorPath",texteditorpath);
    }

    int tbsize(ui->spinBox_iconsize->value());

    if (tbsize != toolbarsize)
        settings->setValue("ToolBarSize",tbsize);

    settings->setValue("ShelXle",shelxlecheck);
    settings->setValue("Olex2",olex2check);
// Save font for QListWidget
    settings->setValue("item.font.family", lwfont.family());
    settings->setValue("item.font.size", lwfont.pointSize());
    settings->setValue("item.font.bold", lwfont.bold());
    settings->setValue("item.font.italic", lwfont.italic());
// End font saving
    settings->sync();
    close();
}

void Settings::addPath1()
{
    QString startDir;

    if (path1.isEmpty())
        startDir = QDir::homePath();
    else
        startDir = path1;

    path1 = QFileDialog::getExistingDirectory(this, tr("Select Indexing Directory"),
                                                    startDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->lineeditPath1->setText(path1);
}

void Settings::addPath2()
{
    QString startDir;

    if (path2.isEmpty())
        startDir = QDir::homePath();
    else
        startDir = path2;

    path2 = QFileDialog::getExistingDirectory(this, tr("Select Indexing Directory"),
                                                    startDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineeditPath2->setText(path2);
}

void Settings::addPath3()
{
    QString startDir;

    if (path3.isEmpty())
        startDir = QDir::homePath();
    else
        startDir = path3;


    path3 = QFileDialog::getExistingDirectory(this, tr("Select Indexing Directory"),
                                                    startDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineeditPath3->setText(path3);
}

void Settings::addDBPath()
{
    pathDB = QFileDialog::getExistingDirectory(this, tr("Select location of the database "),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineEditDB->setText(pathDB);

}

void Settings::addviewer()
{
    viewerpath = QFileDialog::getOpenFileName(this, tr("Viewer Path"),
                                                    QDir::homePath(),
                                                    tr(""));
    ui->lineEdit_viewer->setText(viewerpath);

}

void Settings::addtexteditor()
{
    texteditorpath = QFileDialog::getOpenFileName(this, tr("Text Editor Path"),
                                                    QDir::homePath(),
                                                    tr(""));
    ui->lineEdit_texteditor->setText(texteditorpath);
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

void Settings::deleteviewer()
{
    ui->lineEdit_viewer->clear();
    viewerpath.clear();
    settings->remove("ViewerPath");
}

void Settings::deletetexteditor()
{
    ui->lineEdit_texteditor->clear();
    texteditorpath.clear();
    settings->remove("TextEditorPath");
}

void Settings::shelxlecheckchanged(int i)
{
    if (i == Qt::Checked) {
        shelxlecheck = true;
    } else {
        shelxlecheck = false;
    }
}

void Settings::olex2checkchanged(int i)
{
    if (i == Qt::Checked) {
        olex2check = true;
    } else {
        olex2check = false;
    }
}


void Settings::font_button_clicked()
{
    qDebug("START on_pushButton_font_clicked() function");
    bool ok;
    QFont temp(lwfont);
//    QFontDialog fontdialog(lwfont,this);

    lwfont = QFontDialog::getFont(&ok, temp, this);

    if (ok) {
        ui->pushButton_font->setText(lwfont.family()+", "+QString::number(lwfont.pointSize()));
        emit fontChanged(lwfont);
        qDebug("Call inside IF");
    }

    qDebug("END on_pushButton_font_clicked() function");

}
