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
    alcos = cos(RAD2DEG(_alpha));
    becos = cos(RAD2DEG(_beta));
    gacos = cos(RAD2DEG(_gama));

    alsin = sin(RAD2DEG(_alpha));
    besin = sin(RAD2DEG(_beta));
    gasin = sin(RAD2DEG(_gama));


    _volume = _a*_b*_c*sqrt(1-SQUARE(alcos)-SQUARE(becos)-SQUARE(gacos)+2*alcos*becos*gacos);
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
