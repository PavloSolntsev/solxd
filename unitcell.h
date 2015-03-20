/**************************************************************************
**
** This unitcell.h file is part of SolXd software.
**
** Copyright (C) 2015 Pavlo Solntsev
**
** SolXd is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Foobar is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#ifndef UNITCELL_H
#define UNITCELL_H
#include <QString>
#include <cctbx/uctbx/fast_minimum_reduction.h>

enum FileType{
NONE,
INS,
RES,
CIF
};
enum CellType{
    UNKNOWN = 0,
    PRIMITIVE,
    ICENTERED,
    RHOMBOHIDRAL,
    FCENTERED,
    ACENTERED,
    BCENTERED,
    CCENTERED
};

class Unitcell{
public:

    Unitcell():
        _a(0),
        _b(0),
        _c(0),
        _alpha(0),
        _beta(0),
        _gama(0){} // Default constructor
    Unitcell(double acell,double bcell,double ccell,double alphacell,double betacell,double gamacell);

    virtual ~Unitcell(){}

// Get functions
    inline const double &a()const{return _a;}
    inline const double &b()const{return _b;}
    inline const double &c()const{return _c;}

    inline const double &alpha()const{return _alpha;}
    inline const double &beta()const{return _beta;}
    inline const double &gama()const{return _gama;}

    inline const double &volume(void)const{return _volume;}

    const Unitcell reciprocal(void);
// Set function
    void set_cell(const double &acell,
                  const double &bcell,
                  const double &ccell,
                  const double &alphacell,
                  const double &betacell,
                  const double &gamacell);
    const Unitcell niggli()const;

protected:
    double _a;
    double _b;
    double _c;
    double _alpha;
    double _beta;
    double _gama;
    double _volume;

    double alcos;
    double becos;
    double gacos;

    double alsin;
    double besin;
    double gasin;

// Niggli cell values initialized after reading file from the disk
    double _ra;
    double _rb;
    double _rc;
    double _ralpha;
    double _rbeta;
    double _rgama;

    void sync_data(); // recalculate volume and other numberic parameters if any

};

#endif // UNITCELL_H
