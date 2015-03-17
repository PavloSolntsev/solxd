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
    _cifblock(0)
{
}


Crystfile::Crystfile(FileType type, const QString &path)
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
//    _state = CRGOOD;
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
        if (line.startsWith("END ",Qt::CaseInsensitive)) {
            //sfaccheck = true; // There is no way SFAC group behind this point
            //endcheck = true;
            break;
        }

        if (line.startsWith("HKLF ",Qt::CaseInsensitive)) {
            hklftest = true; // if HKLF instruction is presen VERY high chance this is true RES or INS file
            continue;
        }

        if (line.startsWith("cell ",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            double a(0),b(0),c(0),d(0),e(0),f(0),g(0);
            QString temp;
            // CELL Wavelength cella cellb cellc alpha beta gamma
            buffer >> temp >> g >> a >> b >> c >> d >> e >> f;
            this->_wavelength = g;

            if(     a > 1 &&
                    b > 1 &&
                    c > 1 &&
                    d > 1 &&
                    e > 1 &&
                    f > 1 )
            {
                cellcheck = true;
                set_cell(a,b,c,d,e,f);
                continue;
            }
            else {
                _errors.push_back(CRCELLERORR);
            }
        }

        if (line.startsWith("latt ",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            int a;
            QString temp;
            buffer >> temp >> a;

            if (a>0) {
                _center = true;
            }

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

        if (line.startsWith("sfac ",Qt::CaseInsensitive) && !sfaccheck)
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
                        break;
                    }

                    sfacarray.push_back(a.toLower());
                }
                sfaccheck = true;
                continue;
            }
            else
            { // SFAC with scatering factors
                buffer >> a; // Reading element
                if (a.isEmpty()) {
                    continue;
                }

                sfacarray.push_back(a.toLower());
            }
        }

        if (line.startsWith("unit ",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

//            double a;
            QString temp;

            buffer >> temp;

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

        if(cellcheck && lattcheck && sfaccheck && unitcheck)
        {
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
    bool cellcheck(false);
    QString lineformula;
    int quatcount(0);
    double a(0),b(0),c(0),d(0),e(0),f(0); // unit cell variables

//    int datablockcount(0);

    while(!inp.atEnd()) {

        QString line = inp.readLine();

// Remove spaces at the begining of the line
        while (line.startsWith(' '))
                line.remove(0,1);

        if (line.startsWith("data_"))
        {
            _cifblock++;
            if (_cifblock >1)
                break; // Read only first block from cif file if more them 2 blocks are present.
            else
                continue;
        }

        if (line.startsWith("_cell_length_a",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);
            QString temp;
            buffer >> temp >> a;
//            a = temp.section('(',0,0).toDouble();
//            qDebug() << "Unit A = " << a;
            this->_a = a;
            continue;
        }

        if (line.startsWith("_cell_length_b",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            QString temp;
            buffer >> temp >> b;
//            qDebug() << "Unit B = " << b;
            this->_b = b;
            continue;
        }

        if (line.startsWith("_cell_length_c",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            QString temp;
            buffer >> temp >> c;
//            qDebug() << "Unit C = " << c;
            this->_c = c;
            continue;
        }

        if (line.startsWith("_cell_angle_alpha",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            QString temp;
            buffer >> temp >> d;
//            qDebug() << "Unit D = " << d;
            this->_alpha = d;
            continue;
        }

        if (line.startsWith("_cell_angle_beta",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            QString temp;
            buffer >> temp >> e;
//            qDebug() << "Unit E = " << e;
            this->_beta = e;
            continue;
        }

        if (line.startsWith("_cell_angle_gamma",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            QString temp;
            buffer >> temp >> f;
//            qDebug() << "Unit F = " << f;
            this->_gama = f;
            continue;
        }

        if (!cellcheck) {
            if (_a > 0 &&
                _b > 0 &&
                _c > 0 &&
                _alpha > 0 &&
                _beta > 0 &&
                _gama > 0 ){
                qDebug() << "Before set _a = " << _a << " a = " << a;
//                set_cell(a,b,c,d,e,f);
                qDebug() << "After set  _a = " << _a << " a = " << a;
                cellcheck = true;
                continue;
            }
            else {
                // Error with reading unit cell parameters
                _errors.push_back(CRCELLERORR);
                cellcheck = true;
                continue;
            }
        }

        if (line.startsWith("_diffrn_radiation_wavelength",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            double a;
            QString temp;
            buffer >> temp >> a;
            this->_wavelength = a;
            continue;
        }

        if (line.startsWith("_chemical_formula_sum",Qt::CaseInsensitive)) {
            quatcount = line.count('\'');
//            qDebug() << "quatcount = " << quatcount;
            if (quatcount == 2) {
            // Formula on the same line
                QTextStream buffer(&line.remove('\''));
//                QString temp;

                while(!buffer.atEnd()){
                    QString temp;
                    buffer >> temp;
                    QString number, letter;

                    qDebug() << "Temp = " << temp;

                    for (int var = 0; var < temp.size(); ++var) {
                        if(temp.at(var).isNumber()){
                            number.append(temp.at(var));
                            qDebug() << "1temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                            continue;
                        }

                        if (temp.at(var).isLetter()) {
                            letter.append(temp.at(var));
                            qDebug() << "2temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                            continue;
                        }
                        if (temp.at(var).isPunct())
                            number.append(temp.at(var));
                            qDebug() << "3temp.at(var) at var= " << var << " is: " << temp.at(var) ;

                    }// end for

                    qDebug() << "Done";
                    sfacarray.push_back(letter);
                    unitarray.push_back(number.toDouble());

                    if (sfacarray.size() != unitarray.size()) {
                        qDebug() << "SFAC " << sfacarray.size();
                        qDebug() << "UNIT " << unitarray.size();
                        qDebug("======");
                    }

                }// end while

//                qDebug() << "Line1 is " << line;
                formulasumcheck = false;
                continue;
            }// end if quatcount ==  2

            if (quatcount == 1) {
                // Only part of the formula on the line
                lineformula += line;
                formulasumcheck = true; // Continue reading
//                qDebug() << "Line2 is " << line;
                continue;
            }

            if (quatcount == 0) { // Next line contains all formula
                formulasumcheck = true;
                continue;
            }
        }

        if (formulasumcheck) {
            int i = line.count('\'');
            if ((i+quatcount) < 2) {
                lineformula += line;
                quatcount += i;
                qDebug() << "Lineformula1 is " << lineformula;
                continue;
            }
            else
            {
                formulasumcheck = false;
                lineformula += line;
                quatcount = 0;
                QTextStream buffer(&line.remove('\''));
                QString temp;

                while(!buffer.atEnd()){
                    buffer >> temp;

                    QString number, letter;

                    for (int var = 0; var < temp.size(); ++var) {
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

                        if (temp.at(var).isPunct()) {
                            number.append(temp.at(var));
//                            qDebug() << "3temp.at(var) at var= " << var << " is: " << temp.at(var) ;
                        }

                    }// end for

//                    qDebug() << "Done";
                    sfacarray.push_back(letter);
                    unitarray.push_back(number.toDouble());

                    if (sfacarray.size() != unitarray.size()) {
//                        qDebug() << "Debug in " << __FILE__ << " at " << __LINE__;
//                        qDebug() << "SFAC " << sfacarray.size();
//                        qDebug() << "UNIT " << unitarray.size();
//                        qDebug("======");
                        _errors.push_back(CRFORMULAERROR);
                    }

                }
                continue;

//                qDebug() << "Lineformula2 is " << lineformula;
            }


        } // end if

    } // end while

//    if (_a == 1 || _b == 1 || _c == 1)
//        _state = CRCELLERORR;

//                qDebug() << "Before If a = " << _a;
//                qDebug() << "Before If b = " << _b;
//                qDebug() << "Before If c = " << _c;
//                qDebug() << "Before If d = " << _alpha;
//                qDebug() << "Before If e = " << _beta;
//                qDebug() << "Before If f = " << _gama;

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

bool Crystfile::findCellA(const double &cellA, const double &error)
{
    if(100*qAbs(_a - cellA) < error*_a)
        return true;
    else
        return false;
}

bool Crystfile::findCellB(const double &cellB, const double &error)
{
    if(100*qAbs(_b - cellB) < error*_b)
        return true;
    else
        return false;
}

bool Crystfile::findCellC(const double &cellC, const double &error)
{
    if(100*qAbs(_c - cellC) < error*_c)
        return true;
    else
        return false;
}

bool Crystfile::findCellAlpha(const double &cellAlpha, const double &error)
{
    if(100*qAbs(_alpha - cellAlpha) < error*_alpha)
        return true;
    else
        return false;

}

bool Crystfile::findCellBeta(const double &cellBeta, const double &error)
{
    if(100*qAbs(_beta - cellBeta) < error*_beta)
        return true;
    else
        return false;

}

bool Crystfile::findCellGamma(const double &cellGamma, const double &error)
{
    if(100*qAbs(_gama - cellGamma) < error*_gama)
        return true;
    else
        return false;
}

bool Crystfile::findVolume(const double &vol, const double &error)
{
    double myvol = volume();
    if(100*qAbs(myvol - vol) < error*myvol)
        return true;
    else
        return false;
}

const Unitcell Crystfile::niggli()
{
    Unitcell a;

    if(_a > 0 && _b > 0 && _c > 0 && _alpha > 0 && _beta > 0 && _gama > 0){
    cctbx::uctbx::unit_cell ucell(scitbx::af::double6(_a,_b,_c,_alpha,_beta,_gama));

    cctbx::uctbx::fast_minimum_reduction<double,int> mytest(ucell);

//        qDebug() << "Cell A  = " << mytest.as_unit_cell().parameters().at(0);
//        qDebug() << "Cell B  = " << mytest.as_unit_cell().parameters().at(1);
//        qDebug() << "Cell C  = " << mytest.as_unit_cell().parameters().at(2);
//        qDebug() << "Cell Al = " << mytest.as_unit_cell().parameters().at(3);
//        qDebug() << "Cell Be = " << mytest.as_unit_cell().parameters().at(4);
//        qDebug() << "Cell Ga = " << mytest.as_unit_cell().parameters().at(5);
    }
    return a;
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
        << crfile.error();
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
        >> crfile._errors;

    crfile._type = static_cast<FileType>(a);
    crfile._ctype = static_cast<CellType>(b);
 //   crfile._state = static_cast<Crystfile::CrystfileState>(c);
    crfile.sync_data(); // recalculate volume

    return in;
}
