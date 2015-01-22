#ifndef UNITCELL_H
#define UNITCELL_H

class Unitcell
{
public:
    enum CellType{
        Primitive,
        Acentered,
        Bcentered,
        Ccentered,
        Icentered,
        Fcentered,
        Hexahonal,
        Trihonal,
        Rhombohydral
    };
    enum SpaceType{
        DirectSpace,
        ReciprocalSpace
    };

    Unitcell(double acell, double bcell, double ccell, double alphacell, double betacell, double gammacell, CellType type = Primitive);

    inline double a(void)const{return _acell;}
    inline double b(void)const{return _bcell;}
    inline double c(void)const{return _ccell;}
    inline double alpha(void)const{return _alpha;}
    inline double beta(void)const{return _beta;}
    inline double gama(void)const{return _gama;}

    inline double arp(void)const{return _arpcell;}
    inline double brp(void)const{return _brpcell;}
    inline double crp(void)const{return _crpcell;}
    inline double alpharp(void)const{return _alpharp;}
    inline double betarp(void)const{return _betarp;}
    inline double gamarp(void)const{return _gamarp;}


    inline CellType Brave(void)const{return _type;}
private:
// Direct Space constants
    double _acell;
    double _bcell;
    double _ccell;
    double _alpha;
    double _beta;
    double _gama;


//Reci[rocal space constants
    double _arpcell;
    double _brpcell;
    double _crpcell;
    double _alpharp;
    double _betarp;
    double _gamarp;

    CellType _type;
};

#endif // UNITCELL_H
