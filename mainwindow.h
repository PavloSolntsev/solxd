#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void indexDatabase();
    void runSettings();
    
private:
    Ui::MainWindow *ui;
    Settings *dia;
};

#endif // MAINWINDOW_H
