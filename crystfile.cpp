#include "crystfile.h"


Crystfile::Crystfile()
{



}


Crystfile::Crystfile(Unitcell &cell, QString path):
    Unitcell(cell),
    _path(path)
{
}
