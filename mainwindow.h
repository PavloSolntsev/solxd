#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
#include "unitcell.h"
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    const Unitcell parseINS(const QString &);

public slots:
    void indexDatabase();
    void runSettings();
    void openfile(QListWidgetItem*iteam);

private:
    Ui::MainWindow *ui;
    Settings *dia;



};

#endif // MAINWINDOW_H
