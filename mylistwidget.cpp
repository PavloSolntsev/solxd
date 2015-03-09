#include "mylistwidget.h"
#include <QToolTip>
#include <QEvent>
#include <QHelpEvent>
#include <QListWidgetItem>
#include <crystfile.h>

MyListWidget::MyListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setMouseTracking(true);
}

//bool MyListWidget::event(QEvent *event)
//{
//    if (event->type() == QEvent::ToolTip) {
//        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
//        QListWidgetItem *index;
//        index = NULL;
//        index = itemAt(helpEvent->pos());
//        if (index != NULL) {
//            QToolTip::showText(helpEvent->globalPos(), "Hello World!");
//        } else {
//            QToolTip::hideText();
//            event->ignore();
//        }

//        return true;
//    }
//    return QWidget::event(event);
//}

//void MyListWidget::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);
//    foreach (ShapeItem shapeItem, shapeItems) {
//        painter.translate(shapeItem.position());
//        painter.setBrush(shapeItem.color());
//        painter.drawPath(shapeItem.path());
//        painter.translate(-shapeItem.position());
//    }
//}

//void MyListWidget::enterEvent(QEvent *event)
//{
//    if (event->type() == QEvent::ToolTip) {
//        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
//        QListWidgetItem *index;
//        index = NULL;
//        index = itemAt(helpEvent->pos());
//        if (index != NULL) {
//            QToolTip::showText(helpEvent->globalPos(), "Hello World!");
//        } else {
//            QToolTip::hideText();
//            event->ignore();
//        }
//    }

//}

void MyListWidget::mouseMoveEvent(QMouseEvent *event)
{

    QListWidgetItem *index;
    index = NULL;
    index = itemAt(event->pos());

    if (index != NULL) {
        QVariant *qvr = new QVariant(index->data(Qt::UserRole));
        Crystfile crfile(qvr->value<Crystfile>());
        QString text = QString("Unit Cell: %1 %2 %3 %4 %5 %6").arg(crfile.a()).arg(crfile.b()).arg(crfile.c()).arg(crfile.alpha()).arg(crfile.beta()).arg(crfile.gama());

        QToolTip::showText(QCursor::pos(), text);
    } else {
        QToolTip::hideText();
        event->ignore();
    }

}


