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
