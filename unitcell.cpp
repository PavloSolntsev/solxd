/**************************************************************************
**
** This unitcell.cpp file is part of SolXd software.
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

#include "unitcell.h"
#include "cmath"

#define SQUARE(x) x*x
#define RAD2DEG(x) x*180/M_PI
#define DEG2RAD(x) x*M_PI/180

Unitcell::Unitcell(double acell, double bcell, double ccell, double alphacell, double betacell, double gamacell):
    _a(acell),
    _b(bcell),
    _c(ccell),
    _alpha(alphacell),
    _beta(betacell),
    _gama(gamacell)
{
    alcos = cos(DEG2RAD(_alpha));
    becos = cos(DEG2RAD(_beta));
    gacos = cos(DEG2RAD(_gama));

    alsin = sin(DEG2RAD(_alpha));
    besin = sin(DEG2RAD(_beta));
    gasin = sin(DEG2RAD(_gama));

    _volume = _a*_b*_c*sqrt((1-SQUARE(alcos)-SQUARE(becos)-SQUARE(gacos))+2*(alcos*becos*gacos));

}

const Unitcell Unitcell::reciprocal()
{
    double ra = _b*_c*alsin/_volume;
    double rb = _a*_c*besin/_volume;
    double rc = _a*_b*gasin/_volume;

    double ralcos = (becos*gacos-alcos)/(besin*gasin);
    double rbecos = (alcos*gacos-becos)/(alsin*gasin);
    double rgacos = (alcos*becos-gacos)/(alsin*besin);

    return Unitcell(ra,rb,rc,RAD2DEG(acos(ralcos)),RAD2DEG(acos(rbecos)),RAD2DEG(acos(rgacos)));
}

void Unitcell::set_cell(const double &acell, const double &bcell, const double &ccell, const double &alphacell, const double &betacell, const double &gamacell)
{
    _a = acell;
    _b = bcell;
    _c = ccell;
    _alpha = alphacell;
    _beta = betacell;
    _gama = gamacell;

    alcos = cos(DEG2RAD(_alpha));
    becos = cos(DEG2RAD(_beta));
    gacos = cos(DEG2RAD(_gama));

    alsin = sin(DEG2RAD(_alpha));
    besin = sin(DEG2RAD(_beta));
    gasin = sin(DEG2RAD(_gama));

    _volume = _a*_b*_c*sqrt(1-SQUARE(alcos)-SQUARE(becos)-SQUARE(gacos)+2*alcos*becos*gacos);

}

void Unitcell::sync_data()
{
    alcos = cos(DEG2RAD(_alpha));
    becos = cos(DEG2RAD(_beta));
    gacos = cos(DEG2RAD(_gama));

    alsin = sin(DEG2RAD(_alpha));
    besin = sin(DEG2RAD(_beta));
    gasin = sin(DEG2RAD(_gama));

    _volume = _a*_b*_c*sqrt(1-SQUARE(alcos)-SQUARE(becos)-SQUARE(gacos)+2*alcos*becos*gacos);

}
