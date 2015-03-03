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
    
    struct PathLabel{
        QString path;
        QString label;
    };
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void sync();
    QString dbpath(void)const{return pathDB;}
    QStringList indexpath(void)const{return listpath;}

public slots:
    void addPath1();
    void addPath2();
    void addPath3();
    void addDBPath();

    void deletePath1();
    void deletePath2();
    void deletePath3();
    void deleteDBPath();

    void path1manualchange(const QString &a){path1 = a;}
    void path2manualchange(const QString &a){path2 = a;}
    void path3manualchange(const QString &a){path3 = a;}

    void ok_button_clicked();
    
private:
    Ui::Settings *ui;
    QSettings *settings;

    QString path1;
    QString path2;
    QString path3;
    QString pathDB;

    QStringList listpath;

};

#endif // SETTINGS_H
