#ifndef CRYSTFILE_H
#define CRYSTFILE_H
#include "unitcell.h"
#include <QString>
class Crystfile
{
public:
    Crystfile();

private:
    QString path;
    Unitcell cell;
};

#endif // CRYSTFILE_H
