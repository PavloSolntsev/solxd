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
        QString text = QString("<p style='white-space:pre'><b>Unit Cell:</b> %1 %2 %3 %4 %5 %6").arg(crfile.a()).arg(crfile.b()).arg(crfile.c()).arg(crfile.alpha()).arg(crfile.beta()).arg(crfile.gama());
        text += QString("\n<b>LATT:</b> %1").arg(celltypemap[crfile.getCellType()]);
        text += QString("\n<b>Formula</b>");
        for (int var = 0; var < crfile.getSfacArray().size(); ++var) {

            if (crfile.getSfacArray().at(var).size() == 2) {
                QString sfacstring;
                sfacstring = crfile.getSfacArray().at(var);
                text += QString(" %1 %2").arg(sfacstring.replace(0,2,sfacstring.at(0).toUpper())).arg(crfile.getUnitArray().at(var));
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


