#ifndef CRYSTFILE_H
#define CRYSTFILE_H
#include "unitcell.h"
#include <QString>

class Crystfile : public Unitcell
{
public:
    Crystfile();
    Crystfile(Unitcell &cell, QString path = "");
    void setPath(QString &path){_path=path;}
    void setPath(char *path){_path=path;}

private:
    QString _path;
};

#endif // CRYSTFILE_H
