#include "unitcell.h"

Unitcell::Unitcell(double acell, double bcell, double ccell, double alphacell, double betacell, double gammacell, Unitcell::CellType type):
    _acell(acell),
    _bcell(bcell),
    _ccell(ccell),
    _alpha(alphacell),
    _beta(betacell),
    _gama(gammacell),
    _type(type)
{





}

Cell Unitcell::cell(Unitcell::SpaceType type)
{
    switch(type)
    {
        case DIRECTSPACE:
            return _direc_cell;
        case RESIPROCAL:
            return _rp_cell;
    }
}

double Unitcell::volume()
{
    return
}
