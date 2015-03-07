#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
#include "unitcell.h"
#include <QListWidget>
#include "searchform.h"

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
    void startSearch();
    void outputResults(const QList<Crystfile> &);
    void setToolbarIcons(const int &);
    void popupinformation(QListWidgetItem *item);
    void crystinfowindow(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    Settings *dia;
    SearchForm *sform;
    QString DBpath;




};

#endif // MAINWINDOW_H
