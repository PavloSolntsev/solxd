#ifndef CRYSTFILE_H
#define CRYSTFILE_H
#include "unitcell.h"
#include <QString>
#include <QList>

class Crystfile : public Unitcell
{
public:
    Crystfile();
    Crystfile(FileType type, const QString &path);
//    void setPath(const QString &path){_path=path;}
//    void setPath(const char *path){_path=path;}
    const QString &getPath()const {return _path;}
    const bool &isCenter()const{return _center;}
/*
 * error is a percentage deviation for each unit cell parameter
 * for example, all cell within range cell(1-error) < cell < cell(1+error)
 *
*/
    bool findCell(const Unitcell &cell, const double &error);
    bool findWL(const double &wl);
    bool findSfac(const QString &sfac);
    bool findBrave(const CellType &ctype);


private:
    QString _path;
    FileType _type;
    CellType _ctype;

    double _wavelength;
    bool _center;
    QList<QString> sfacarray;
    QList<double> unitarray;

    void parseINS();
    void parseCIF();
};

#endif // CRYSTFILE_H
