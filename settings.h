#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT
    
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

public slots:
    void addPath1();
    void addPath2();
    void addPath3();
    void addPath4();
    void addPath5();
    
private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
