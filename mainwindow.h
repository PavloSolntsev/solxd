/**************************************************************************
**
** This mainwindow.h file is part of SolXd software.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
#include "unitcell.h"
#include <QListWidget>
#include "searchform.h"
#include "mylistwidget.h"
#include <QToolTip>
#include <QTextEdit>
#include <QDockWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
//    const Unitcell parseINS(const QString &);

public slots:
    void indexDatabase();
    void runSettings();
    void openfile(QListWidgetItem*iteam);
    void startSearch();
    void outputResults(const QList<Crystfile> &);
    void setToolbarIcons(const int &);
    void crystinfowindow(QListWidgetItem *item);
    void checktime();
    void openlistwidgetfiles();
    void openbrowsfiles();
    void openfilesastext();
    void deleteselectedfiles();
    void changelwfont(const QFont &font);
    void displaymassage(const QString &);
    void aboutQt();
    void help();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private:
    Ui::MainWindow *ui;
    Settings *dia;
    SearchForm *sform;
    QString DBpath;
    QMenu *contextmenu;
    QAction *aopenfiles,*browsfiles,*openastext, *deletefiles;
    QTextEdit* helpdialog;
    QDockWidget *dock;
    QAction *searchw;
};

#endif // MAINWINDOW_H
