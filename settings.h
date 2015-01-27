#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QString>
#include "solXd.h"

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

public slots:
    void addPath1();
    void addPath2();
    void addPath3();

    void deletePath1();
    void deletePath2();
    void deletePath3();

    void ok_button_clicked();
    
private:
    Ui::Settings *ui;
    QSettings *settings;

    QString path1;
    QString path2;
    QString path3;

};

#endif // SETTINGS_H
