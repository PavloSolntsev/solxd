/**************************************************************************
**
** This crystfile.cpp file is part of SolXd software.
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

#include "crystfile.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDebug>


Crystfile::Crystfile():
    _path(""),
    _type(NONE),
    _ctype(UNKNOWN),
    _wavelength(0),
    _cifblock(0),
    _center(false)
{
}

Crystfile::Crystfile(FileType type, const QString &path):Unitcell()
{
    _type = type;
    _path = path;
    _center = false;
    _cifblock = 0;

    switch (_type) {
    case INS:
    case RES:
//        qDebug() << "Before parseINS";
//        qDebug() << "Path is " << _path;
        parseINS();
        break;
    case CIF:
        parseCIF();
        break;
    default:
        break;
    }

}

bool Crystfile::findCell(const Unitcell &cell, const double &error)
{
    bool res(false);

    if(cell.a()*(1-error) <= cell.a() && cell.a()*(1+error) >= cell.a() )
        res = true;
    else
    {
        res = false;
        return res;
    }

    if(cell.b()*(1-error) <= cell.b() && cell.b()*(1+error) >= cell.b() )
        res = true;
    else
    {
        res = false;
        return res;
    }

    if(cell.c()*(1-error) <= cell.c() && cell.c()*(1+error) >= cell.c() )
        res = true;
    else
    {
        res = false;
        return res;
    }

    if(cell.alpha()*(1-error) <= cell.alpha() && cell.alpha()*(1+error) >= cell.alpha() )
        res = true;
    else
    {
        res = false;
        return res;
    }

    if(cell.beta()*(1-error) <= cell.beta() && cell.beta()*(1+error) >= cell.beta() )
        res = true;
    else
    {
        res = false;
        return res;
    }
    if(cell.gama()*(1-error) <= cell.gama() && cell.gama()*(1+error) >= cell.gama() )
        res = true;
    else
    {
        res = false;
        return res;
    }

    return res;

}

bool Crystfile::findSfac(const QString &sfac)
{
    if(sfacarray.contains(sfac.toLower()))
        return true;
    else
        return false;
}

void Crystfile::parseINS()
{
    QFile file(_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        switch (file.error()) {
        case QFile::ReadError:
        case QFile::FatalError:
        case QFile::OpenError:
        case QFile::PermissionsError:
            QMessageBox::information(0, "error", file.errorString());
            _errors.push_back(CRFILEOPENERROR);
            break;
        default:
            break;
        }
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream inp(&file);

    bool cellcheck(false);
    bool lattcheck(false);
    bool sfaccheck(false);
    bool unitcheck(false);
    bool hklftest(false);
//    bool endcheck(false);

    while(!inp.atEnd()) {
        QString line = inp.readLine();
//        qDebug() << "ParseINS \n" << line;
        if (line.startsWith("END ",Qt::CaseInsensitive))
            break;

        if (line.startsWith("HKLF ",Qt::CaseInsensitive)) {
            hklftest = true; // if HKLF instruction is presen VERY high chance this is true RES or INS file
            continue;
        }

        if (line.startsWith("cell ",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

//            double a(0),b(0),c(0),d(0),e(0),f(0),g(0);
            QString temp;
            // CELL Wavelength cella cellb cellc alpha beta gamma
            buffer >> temp >> this->_wavelength >> _a >> _b >> _c >> _alpha >> _beta >> _gama;
//            this->_wavelength = g;

            if(     _a > 0 &&
                    _b > 0 &&
                    _c > 0 &&
                    _alpha > 0 &&
                    _beta > 0 &&
                   _gama > 0 )
            {
                cellcheck = true;
//                set_cell(a,b,c,d,e,f);
                continue;
            }
            else
                _errors.push_back(CRCELLERORR);

        }

        if (line.startsWith("latt ",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            int a;
            QString temp;
            buffer >> temp >> a;

            if (a>0)
                _center = true;

            a = qAbs(a);

            switch (a) {
            case 1:
                this->_ctype = PRIMITIVE;
                break;
            case 2:
                this->_ctype = ICENTERED;
                break;
            case 3:
                this->_ctype = RHOMBOHIDRAL;
                break;
            case 4:
                this->_ctype = FCENTERED;
                break;
            case 5:
                this->_ctype = ACENTERED;
                break;
            case 6:
                this->_ctype = BCENTERED;
                break;
            case 7:
                this->_ctype = CCENTERED;
                break;
            default:
                _errors.push_back(CRLATTERROR);
                break;
            }
            lattcheck = true;
            continue;
        }

        if (line.startsWith("sfac ",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString a;
            buffer >> a; // Reading SFAC keyword

            if (!line.contains('=')) { // Normal SFAC  C Cu CL N O
                while (!buffer.atEnd()) {
                    buffer >> a;

                    if (a.isEmpty())
                        continue;
                    if (a == "?") {
                        sfaccheck = true;
                        _errors.push_back(CRFORMULAERROR);
                        break;
                    }

                    sfacarray.push_back(a.toLower());
                }
                sfaccheck = true;
                continue;
            }
            else
            { // SFAC with scatering factors
//                qDebug() << "SFAC debug " << _path;
//                qDebug() << line;
                buffer >> a; // Reading element
//                qDebug() << "a: " << a;
                if (a.isEmpty()) {
                    continue;
                }

                sfacarray.push_back(a.toLower());
//                sfaccheck = true;
            }
        }

        if (line.startsWith("unit ",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

//            double a;
            QString temp;

            buffer >> temp; // Reading UNIT keyword

            while (!buffer.atEnd()) {
                buffer >> temp;
                if (temp == "?") {
                    _errors.push_back(CRFORMULAERROR);
                    continue;
                }

                unitarray.push_back(temp.toDouble());
            }

            unitcheck = true;
            continue;
        }

        if (sfacarray.size()>0 && unitcheck) {
            sfaccheck = true;
        }

        if(cellcheck && lattcheck && sfaccheck && unitcheck)
        {

            if(!_errors.contains(CRFORMULAERROR)){
                int index = unitarray.indexOf(0);
                while(index > 0)
                {
                    unitarray.removeAt(index);
                    sfacarray.removeAt(index);
                    index = unitarray.indexOf(0);
                }

                if(unitarray.size() != sfacarray.size())
                    _errors.push_back(CRFORMULAERROR);
                continue;

            }
        }
    }

    if(!hklftest)
        _errors.push_back(CRINSERROR);

    file.close();
} // end res/ins pareser

void Crystfile::parseCIF()
{
//    qDeug() << "Parsing CIF file" << _path;
    QFile file(_path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        switch (file.error()) {
        case QFile::ReadError:
        case QFile::FatalError:
        case QFile::OpenError:
        case QFile::PermissionsError:
            QMessageBox::information(0, "error", file.errorString());
            _errors.push_back(CRFILEOPENERROR);
            break;
        default:
            break;
        }
    }

    QTextStream inp(&file);
    bool formulasumcheck(false);
    int cellcount(0); // count the number of unit cell constants, If 6, then all constants have been read
    QString lineformula;
    int quatcount(0);

    while(!inp.atEnd()) {
        QString line = inp.readLine();

// Remove spaces at the begining of the line
        while (line.startsWith(' '))
                line.remove(0,1);

        if (line.startsWith("_cell_length_a",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString temp;
            buffer >> temp >> this->_a;
            cellcount++;
            continue;
        }

        if (line.startsWith("_cell_length_b",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString temp;
            buffer >> temp >> this->_b;
            cellcount++;
            continue;
        }

        if (line.startsWith("_cell_length_c",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString temp;
            buffer >> temp >> this->_c;
            cellcount++;
            continue;
        }

        if (line.startsWith("_cell_angle_alpha",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString temp;
            buffer >> temp >> this->_alpha;
            cellcount++;
            continue;
        }

        if (line.startsWith("_cell_angle_beta",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString temp;
            buffer >> temp >> this->_beta;
            cellcount++;
            continue;
        }

        if (line.startsWith("_cell_angle_gamma",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString temp;
            buffer >> temp >> this->_gama;
            cellcount++;
            continue;
        }

        if (cellcount == 6) {
            if (_a > 0 &&
                _b > 0 &&
                _c > 0 &&
                _alpha > 0 &&
                _beta > 0 &&
                _gama > 0 ){
                cellcount++;
                continue;
            }
            else {
                // Error with reading unit cell parameters
                _errors.push_back(CRCELLERORR);
                qDebug("Unit cell ERROR");
                qDebug() << _path;
                qDebug() << "a = " << _a;
                qDebug() << "b = " << _b;
                qDebug() << "c = " << _c;
                qDebug() << "alpha = " << _alpha;
                qDebug() << "beta = " << _beta;
                qDebug() << "gamma = " << _gama;
                cellcount++;
                continue;
            }
        }

        if (line.startsWith("_diffrn_radiation_wavelength",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString temp;
            buffer >> temp >> this->_wavelength;
            continue;
        }

        if (line.startsWith("_chemical_formula_sum",Qt::CaseInsensitive)) {
            quatcount = line.count('\'');
            if (quatcount == 2) {
            // Formula on the same line
                QTextStream buffer(&line.remove('\''));
                QString temp;

                buffer >> temp; // reading "_chemical_formula_sum"

                while(!buffer.atEnd()){
//                    QString temp;
                    buffer >> temp;
                    QString number, letter;

                    for (int var = 0; var < temp.size(); ++var) {
                        if (temp.at(0).isLetter() && temp.size() == 1) {
                            letter.append(temp.at(0));
                            number.append('0');
                            continue;
                        }

                        if(temp.at(var).isNumber()){
                            number.append(temp.at(var));
//                            qDebug() << "1temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                            continue;
                        }

                        if (temp.at(var).isLetter()) {
                            letter.append(temp.at(var));
//                            qDebug() << "2temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                            continue;
                        }
                        if (temp.at(var).isPunct())
                            number.append(temp.at(var));
//                            qDebug() << "3temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                    }// end for

                    sfacarray.push_back(letter);
                    unitarray.push_back(number.toDouble());

                    if (sfacarray.size() != unitarray.size()) {
                        _errors.push_back(CRFORMULAERROR);
                    }

                }// end while

//                formulasumcheck = false;
                continue;
            }// end if quatcount ==  2

            if (quatcount == 1) {
                // Only part of the formula on the line
                lineformula += line;
                formulasumcheck = true; // Continue reading
                continue;
            }

            if (quatcount == 0) { // Next line contains all formula
                formulasumcheck = true;
                continue;
            }
        }

        if (formulasumcheck) {
            quatcount += line.count('\'');
            if (quatcount < 2) {
                lineformula += line;
                qDebug() << "Lineformula1 is " << lineformula;
                continue;
            }
            else
            {// All formula line has been read
                formulasumcheck = false;
                lineformula += line;
                quatcount = 0;
                QTextStream buffer(&lineformula.remove('\''));
                QString temp;

                buffer >> temp; // reading _chemical_formula_sum

                while(!buffer.atEnd()){
                    buffer >> temp;

                    QString number, letter;

                    for (int var = 0; var < temp.size(); ++var) {
                        if (temp.at(0).isLetter() && temp.size() == 1) {
                            letter.append(temp.at(0));
                            number.append('0');
                            continue;
                        }

                        if(temp.at(var).isNumber()){
                            number.append(temp.at(var));
//                            qDebug() << "4temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                            continue;
                        }

                        if (temp.at(var).isLetter()) {
                            letter.append(temp.at(var));
//                            qDebug() << "5temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                            continue;
                        }

                        if (temp.at(var).isPunct()) {
                            number.append(temp.at(var));
//                            qDebug() << "6temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                        }

                    }// end for

                    sfacarray.push_back(letter);
                    unitarray.push_back(number.toDouble());

                    if (sfacarray.size() != unitarray.size()) {
                        _errors.push_back(CRFORMULAERROR);
                    }

                }
                continue;

//                qDebug() << "Lineformula2 is " << lineformula;
            }


        } // end if

        if (cellcount > 6 && _wavelength > 0 && unitarray.size() > 0 && sfacarray.size() > 0) {
            break;
        }

    } // end while

    if (sfacarray.size() == 0 || unitarray.size() == 0) {
        _errors.push_back(CRFORMULAERROR);
    }

    if (_a < 1 || _b < 1 || _c < 1 || _alpha < 1 || _beta < 1 || _gama < 1)  {
        _errors.push_back(CRCELLERORR);
    }

    sync_data(); // Sync all internal numeric parameters
    file.close();
} // end cifparser

bool Crystfile::findBrave(const CellType &ctype)
{
    if (ctype == _ctype)
        return true;
    else
        return false;
}

bool Crystfile::findWL(const double &wl, const double &error)
{
    if(100*qAbs(_wavelength - wl) < error*_wavelength)
        return true;
    else
        return false;
}

bool Crystfile::findCellA(const double &cellA, const double &error,const bool &niggli)
{
//    if (_a != _ra) {
//        qDebug() <<"A cells are different for normal and reduced cells";
//        qDebug() << "_a = " << _a << " _ra = " << _ra;
//    }
    if(100*qAbs(_a - cellA) < error*_a)
        return true;
    else{
        if(niggli){
            if (100*qAbs(_ra - cellA) < error*_ra)
                return true;
            else
                return false;
        }
        else
            return false;
    }
}

bool Crystfile::findCellB(const double &cellB, const double &error,const bool &niggli)
{
//    if (_b != _rb) {
//        qDebug() <<"A cells are different for normal and reduced cells";
//        qDebug() << "_b = " << _b << " _rb = " << _rb;
//    }

    if(100*qAbs(_b - cellB) < error*_b)
        return true;
    else{
        if(niggli){
            if(100*qAbs(_rb - cellB) < error*_rb)
                return true;
            else
                return false;
        }
        else
            return false;
    }
}

bool Crystfile::findCellC(const double &cellC, const double &error, const bool &niggli)
{
    if(100*qAbs(_c - cellC) < error*_c)
        return true;
    else{
        if(niggli){
            if(100*qAbs(_rc - cellC) < error*_rc)
                return true;
            else
                return false;
        }
        else
            return false;
    }
}

bool Crystfile::findCellAlpha(const double &cellAlpha, const double &error, const bool &niggli)
{
//    if (_alpha != _ralpha) {
//        qDebug() <<"A cells are different for normal and reduced cells";
//        qDebug() << "_alpha = " << _alpha << " _raalpha = " << _ralpha;
//    }

    if(100*qAbs(_alpha - cellAlpha) < error*_alpha)
        return true;
    else{
        if(niggli){
            if(100*qAbs(_ralpha - cellAlpha) < error*_ralpha)
                return true;
            else
                return false;
        }
        else
            return false;
    }
}

bool Crystfile::findCellBeta(const double &cellBeta, const double &error, const bool &niggli)
{
//    if (qAbs(_beta - _rbeta) > 0.0001) {
//        if (qAbs(_alpha - _ralpha) > 0.0001){
//            qDebug() <<"A cells are different for normal and reduced cells";
//            qDebug() << "_alpha = " << _alpha << " _ralpha = " << _ralpha;
//            qDebug() << "_beta  = " << _beta << " _rbeta = " << _rbeta;
//            qDebug() << "From file: " << _path;
//            qDebug() << "====================================================";
//        }
//    }

    if(100*qAbs(_beta - cellBeta) < error*_beta)
        return true;
    else{
        if(niggli){
            if(100*qAbs(_rbeta - cellBeta) < error*_rbeta)
                return true;
            else
                return false;
        }
        else
            return false;
    }
}

bool Crystfile::findCellGamma(const double &cellGamma, const double &error, const bool &niggli)
{
//    if (_gama != _rgama) {
//        qDebug() <<"A cells are different for normal and reduced cells";
//        qDebug() << "_gama = " << _gama << " _rgama = " << _rgama;
//    }

    if(100*qAbs(_gama - cellGamma) < error*_gama)
        return true;
    else{
        if(niggli){
            if(100*qAbs(_rgama - cellGamma) < error*_rgama)
                return true;
            else
                return false;
        }
        else
            return false;
    }
}

bool Crystfile::findCellABC(const double &Par, const double &error,const bool &niggli)
{
    if(findCellA(Par,error,niggli))
        return true;
    else{
        if(findCellB(Par,error,niggli))
            return true;
        else{
            if(findCellC(Par,error,niggli))
                return true;
            else
                return false;
        }
    }
}

bool Crystfile::findAngleABC(const double &Par, const double &error,const bool &niggli)
{
    if(findCellAlpha(Par,error,niggli))
        return true;
    else{
        if(findCellBeta(Par,error,niggli))
            return true;
        else{
            if(findCellGamma(Par,error,niggli))
                return true;
            else
                return false;
        }
    }
}

bool Crystfile::findVolume(const double &vol, const double &error)
{
    double myvol = volume();
    if(100*qAbs(myvol - vol) < error*myvol)
        return true;
    else
        return false;
}


QDataStream &operator<<(QDataStream &out, const Crystfile::CrystfileErrors &error)
{
   out << (int)error;
   return out;
}

QDataStream &operator>>(QDataStream &in, Crystfile::CrystfileErrors &error)
{
    int a;
    in >> a;
    error = static_cast<Crystfile::CrystfileErrors>(a);
    return in;
}

QDataStream &operator <<(QDataStream &out, const Crystfile &crfile)
{
    out << crfile._path
        << (int)crfile._type
        << crfile._ctype
        << crfile._wavelength
        << crfile._center
        << crfile.sfacarray
        << crfile.unitarray
        << crfile.a()
        << crfile.b()
        << crfile.c()
        << crfile.alpha()
        << crfile.beta()
        << crfile.gama()
        << crfile.error()
        << crfile.niggli().a()
        << crfile.niggli().b()
        << crfile.niggli().c()
        << crfile.niggli().alpha()
        << crfile.niggli().beta()
        << crfile.niggli().gama();
    return out;
}

QDataStream &operator >>(QDataStream &in, Crystfile &crfile)
{
    int a,b;
    in >> crfile._path
        >> a
        >> b
        >> crfile._wavelength
        >> crfile._center
        >> crfile.sfacarray
        >> crfile.unitarray
        >> crfile._a
        >> crfile._b
        >> crfile._c
        >> crfile._alpha
        >> crfile._beta
        >> crfile._gama
        >> crfile._errors
        >> crfile._ra
        >> crfile._rb
        >> crfile._rc
        >> crfile._ralpha
        >> crfile._rbeta
        >> crfile._rgama;

    crfile._type = static_cast<FileType>(a);
    crfile._ctype = static_cast<CellType>(b);
 //   crfile._state = static_cast<Crystfile::CrystfileState>(c);
    crfile.sync_data(); // recalculate volume

    return in;
}
