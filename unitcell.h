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
    inline double a()const{return _a;}
    inline double b()const{return _b;}
    inline double c()const{return _c;}

    inline double alpha()const{return _alpha;}
    inline double beta()const{return _beta;}
    inline double gama()const{return _gama;}

    inline double volume(void)const{return _volume;}

    const Unitcell reciprocal(void);

// Set functions
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

};
/*
class Unitcell : public Cell
{
public:
    enum CellType{
        PRIMITIVE,
        ACEMTERED,
        BCENTERED,
        CCENTERED,
        ICENTERED,
        FCENTERED,
        HEXAGONAL,
        TRIGONAL,
        RHOMBOHIDRAL
    };
    Unitcell();
    Unitcell(Cell &cell, CellType type = PRIMITIVE);

    inline CellType Brave(void)const{return _type;}

private:
// Cell type
    CellType _type;
};
*/
#endif // UNITCELL_H
