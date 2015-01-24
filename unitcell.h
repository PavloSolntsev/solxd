#ifndef UNITCELL_H
#define UNITCELL_H

class Cell{
public:

    Cell():_a(1),_b(1),_c(1),_alpha(90),_beta(90),_gama(90){} // Default constructor
    Cell(double acell,double bcell,double ccell,double alphacell,double betacell,double gamacell);
    virtual ~Cell(){}

    inline double a()const{return _a;}
    inline double b()const{return _b;}
    inline double c()const{return _c;}

    inline double alpha()const{return _alpha;}
    inline double beta()const{return _beta;}
    inline double gama()const{return _gama;}

    inline double volume(void)const{return _volume;}

    Cell reciprocal(void);

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

};

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

#endif // UNITCELL_H
