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
    ACEMTERED,
    BCENTERED,
    CCENTERED,
    ICENTERED,
    FCENTERED,
    HEXAGONAL,
    TRIGONAL,
    RHOMBOHIDRAL
};



class Unitcell{
public:

    Unitcell():_a(1),_b(1),_c(1),_alpha(90),_beta(90),_gama(90),_type(UNKNOWN){} // Default constructor
    Unitcell(double acell,double bcell,double ccell,double alphacell,double betacell,double gamacell,CellType type=PRIMITIVE);

    virtual ~Unitcell(){}

// Get functions
    inline double a()const{return _a;}
    inline double b()const{return _b;}
    inline double c()const{return _c;}

    inline double alpha()const{return _alpha;}
    inline double beta()const{return _beta;}
    inline double gama()const{return _gama;}

    inline double volume(void)const{return _volume;}

    const Unitcell &reciprocal(void);
    inline CellType Brave(void)const{return _type;}

// Set functions
    void setCellType(CellType type){_type = type;}



private:
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

    CellType _type;

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
