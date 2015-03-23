/**************************************************************************
**
** This mylistwidget.cpp file is part of SolXd software.
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

#include "mylistwidget.h"
#include <QToolTip>
#include <QEvent>
#include <QHelpEvent>
#include <QListWidgetItem>
#include <crystfile.h>
#include <QDebug>

MyListWidget::MyListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setMouseTracking(true);
}

void MyListWidget::mouseMoveEvent(QMouseEvent *event)
{
    QListWidgetItem *index;
    index = NULL;
    index = itemAt(event->pos());

    if (index != NULL) {
        QMap<CellType,QString> celltypemap;
        celltypemap[PRIMITIVE] = "Primitive";
        celltypemap[ACENTERED] = "A-Centered";
        celltypemap[BCENTERED] = "B-Centered";
        celltypemap[CCENTERED] = "C-Centered";
        celltypemap[ICENTERED] = "I-Centered";
        celltypemap[FCENTERED] = "F-Centered";
        celltypemap[RHOMBOHIDRAL] = "Rhombohidral";

        QVariant *qvr = new QVariant(index->data(Qt::UserRole));
        Crystfile crfile(qvr->value<Crystfile>());

//        qDebug() << "Under mouse A = " << crfile.a();
        QString text = QString("<p style='white-space:pre'>");
        text += QString("<b>Unit Cell:</b> a= %1\tb= %2\tc= %3").arg(crfile.a()).arg(crfile.b()).arg(crfile.c());
        text += QString("\n\t%1= %2\t%3= %4\t%5= %6").arg(QChar(0x03B1)).arg(crfile.alpha()).arg(QChar(0x03B2)).arg(crfile.beta()).arg(QChar(0x03B3)).arg(crfile.gama());
        text += QString("\n<b>LATT:</b> %1").arg(celltypemap[crfile.getCellType()]);
        text += QString("\n<b>Center:</b> ");
        if (crfile.isCenter())
            text += "Yes";
        else
            text += "No";
        text += QString("\n<b>Formula</b>");
        for (int var = 0; var < crfile.getSfacArray().size(); ++var) {
            if (crfile.getSfacArray().at(var).size() == 2) {
                QString sfacstring(crfile.getSfacArray().at(var));
                text += QString(" %1 %2").arg(sfacstring.replace(0,1,sfacstring.at(0).toUpper())).arg(crfile.getUnitArray().at(var));
//                qDebug() << "Before :" << sfacstring << " After :" << sfacstring.replace(0,2,sfacstring.at(0).toUpper());

            }

            if(crfile.getSfacArray().at(var).size() == 1)
            {
                text += QString(" %1 %2").arg(crfile.getSfacArray().at(var).toUpper()).arg(crfile.getUnitArray().at(var));

            }
        }

        text += QString("\n<b>Volume:</b> %1").arg(crfile.volume());


        QToolTip::showText(QCursor::pos(), text);
    } else {
        QToolTip::hideText();
        event->ignore();
    }

}


