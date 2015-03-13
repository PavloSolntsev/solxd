/**************************************************************************
**
** This mylistwidget.h file is part of SolXd software.
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

#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>
#include <QEvent>
#include <QPaintEvent>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *event);

    
signals:
    
public slots:
    
};

#endif // MYLISTWIDGET_H
