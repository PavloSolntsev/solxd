#ifndef UNITCELL_H
#define UNITCELL_H
#include <QString>

enum FileType{
NONE,
INS,
RES,
CIF
};
enum CellType{
    UNKNOWN,
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

    Unitcell():_a(1),_b(1),_c(1),_alpha(90),_beta(90),_gama(90){} // Default constructor
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
    void sync_volume();

};

#endif // UNITCELL_H
