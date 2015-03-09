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
    const int &getToolbarSize(void)const{return toolbarsize;}
    bool excludeshelxlefolders(void)const{return shelxlecheck;}
    const QString &getViewer(void)const{return viewerpath;}

public slots:
    void addPath1();
    void addPath2();
    void addPath3();
    void addDBPath();
    void addviewer();

    void deletePath1();
    void deletePath2();
    void deletePath3();
    void deleteDBPath();
    void deleteviewer();

    void path1manualchange(const QString &a){path1 = a;}
    void path2manualchange(const QString &a){path2 = a;}
    void path3manualchange(const QString &a){path3 = a;}
    void shelxlecheckchanged(int i);
    void viewerpathchanged(const QString &a){viewerpath = a;}

    void ok_button_clicked();
signals:
    void toolbarIconsChanged(const int &);

private:
    Ui::Settings *ui;
    QSettings *settings;

    QString path1;
    QString path2;
    QString path3;
    QString pathDB;
    QString viewerpath;

    QStringList listpath;
    bool shelxlecheck;

    int toolbarsize;

};

#endif // SETTINGS_H
