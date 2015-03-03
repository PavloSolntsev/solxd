#ifndef CRYSTFILE_H
#define CRYSTFILE_H
#include "unitcell.h"
#include <QString>
#include <QList>
//#include <cctbx/uctbx/fast_minimum_reduction.h>

class Crystfile : public Unitcell
{
public:
    Crystfile();
    Crystfile(FileType type, const QString &path);
//    void setPath(const char *path){_path=path;}
/*
 * error is a percentage deviation for each unit cell parameter
 * for example, all cell within range cell(1-error) < cell < cell(1+error)
 *
*/
    bool findCell(const Unitcell &cell, const double &error);
    bool findWL(const double &wl);
    bool findSfac(const QString &sfac);
    bool findBrave(const CellType &ctype);

    const QString &getPath()const {return _path;}
    const FileType &getFileType()const{return _type;}
    const CellType &getCellType()const{return _ctype;}
    const double &getWavelength()const{return _wavelength;}
    const bool &isCenter()const{return _center;}
    const QList<QString> &getSfacArray()const{return sfacarray;}
    const QList<double> &getUnitArray()const{return unitarray;}

    void setPath(const QString &path){_path=path;}
    void setFileType(const FileType &a){_type = a;}
    void setCellType(const CellType &a){_ctype = a;}
    void setWavelength(const double &a){_wavelength = a;}
    void setCenter(const bool &a){_center = a;}
    void setSfacArray(const QList<QString> &a){sfacarray = a;}
    void setUnitArray(const QList<double> &a){unitarray = a;}

    friend QDataStream& operator<<(QDataStream& out, const Crystfile& crfile);
    friend QDataStream& operator>>(QDataStream& in, Crystfile& crfile);

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
