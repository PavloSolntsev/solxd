#ifndef UNITCELL_H
#define UNITCELL_H

class Cell{
public:
    Cell():a(1),b(1),c(1),alpha(90),beta(90),gama(90){}

    double a()const{return _a;}
    double b()const{return _b;}
    double c()const{return _c;}

    double alpha()const{return _alpha;}
    double beta()const{return _beta;}
    double gama()const{return _gama;}


private:
    double _a;
    double _b;
    double _c;
    double _alpha;
    double _beta;
    double _gama;
};



class Unitcell
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
    enum SpaceType{
        DIRECTSPACE,
        RESIPROCAL
    };

    Unitcell(double acell, double bcell, double ccell, double alphacell, double betacell, double gammacell, CellType type = Primitive);

    Cell cell(SpaceType type = DIRECTSPACE);
    double volume(void);

    inline CellType Brave(void)const{return _type;}


private:
// Direct Space constants
    Cell _direc_cell;

//Reci[rocal space constants
    Cell _rp_cell;
// Cell type
    CellType _type;
};

#endif // UNITCELL_H
