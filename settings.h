/**************************************************************************
**
** This settings.h file is part of SolXd software.
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QString>
#include "solXd.h"
#include <QFont>
#include <QDebug>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT
    
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void sync();
    const QString &dbpath(void)const{return pathDB;}
    QStringList indexpath(void)const{return listpath;}
    const int &getToolbarSize(void)const{return toolbarsize;}
    const bool &excludeshelxlefolders(void)const{return shelxlecheck;}
    const bool &excludeolex2folders(void)const{return olex2check;}
    const QString &getViewer(void)const{return viewerpath;}
    const QString &getTextEditor()const{return texteditorpath;}
    const QFont &get_lwfont()const{return lwfont;}

public slots:
    void addPath1();
    void addPath2();
    void addPath3();
    void addDBPath();
    void addviewer();
    void addtexteditor();

    void deletePath1();
    void deletePath2();
    void deletePath3();
    void deleteDBPath();
    void deleteviewer();
    void deletetexteditor();

    void path1manualchange(const QString &a){path1 = a;}
    void path2manualchange(const QString &a){path2 = a;}
    void path3manualchange(const QString &a){path3 = a;}
    void shelxlecheckchanged(int i);
    void olex2checkchanged(int i);
    void viewerpathchanged(const QString &a){viewerpath = a;}
    void texteditorchanged(const QString &a){texteditorpath = a;}
    void ok_button_clicked();
    void font_button_clicked();

signals:
    void toolbarIconsChanged(const int &);
    void fontChanged(const QFont &);

private:
    Ui::Settings *ui;
    QSettings *settings;

    QString path1;
    QString path2;
    QString path3;
    QString pathDB;
    QString viewerpath;
    QString texteditorpath;

    QStringList listpath;
    bool shelxlecheck;
    bool olex2check;
    QFont lwfont;

    int toolbarsize;

};

#endif // SETTINGS_H
