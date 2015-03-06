#include "crystfile.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDebug>


Crystfile::Crystfile():
    _path(""),
    _type(NONE),
    _ctype(UNKNOWN)
{
    _wavelength = 0;


}


Crystfile::Crystfile(FileType type, const QString &path)
{
    _type = type;
    _path = path;
    _center = false;

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
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream inp(&file);

    bool cellcheck(false);
    bool lattcheck(false);
    bool sfaccheck(false);
    bool unitcheck(false);

    while(!inp.atEnd()) {
        QString line = inp.readLine();
//        qDebug() << "ParseINS \n" << line;

        if (line.startsWith("cell",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

            double a,b,c,d,e,f,g;
            QString temp;
            buffer >> temp >> g >> a >> b >> c >> d >> e >> f;
            this->_wavelength = g;
            this->_a = a;
            this->_b = b;
            this->_c = c;
            this->_alpha = d;
            this->_beta = e;
            this->_gama = f;

            cellcheck = true;
            continue;
        }

        if (line.startsWith("latt",Qt::CaseInsensitive))
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
                break;
            }
            lattcheck = true;
            continue;
        }

        if (line.startsWith("sfac",Qt::CaseInsensitive) && !sfaccheck)
        {
            QTextStream buffer(&line);
            QString a;
            buffer >> a; // Reading SFAC keyword
            //            QString temp;

            if (!line.contains('=')) { // Normal SFAC  C Cu CL N O
                while (!buffer.atEnd()) {
                    buffer >> a;

                    if (a.isEmpty()) {
                        continue;
                    }
                    if (a == "?") {
                        sfaccheck = true;
                        break;
                    }
//                    qDebug() << "reading SFAC " << a;

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

        if (line.startsWith("END",Qt::CaseInsensitive)) {
            sfaccheck = true; // There is no way SFAC group behind this point
        }

        if (line.startsWith("unit",Qt::CaseInsensitive))
        {
            QTextStream buffer(&line);

//            double a;
            QString temp;

            buffer >> temp;

            while (!buffer.atEnd()) {
                buffer >> temp;
                if (temp == "?") {
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

            break;
        }
    }

    file.close();
}

void Crystfile::parseCIF()
{
    QFile file(_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream inp(&file);

    while(!inp.atEnd()) {
        QString line = inp.readLine();

        if (line.indexOf("_cell_length_a",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a;
            QString temp;
            buffer >> temp >> a;
            this->_a = a;
//            qDebug() << "ParseCIF line is:\n" << line;
        }

        if (line.indexOf("_cell_length_b",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a;
            QString temp;
            buffer >> temp >> a;
            this->_b = a;
        }

        if (line.indexOf("_cell_length_c",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a;
            QString temp;
            buffer >> temp >> a;
            this->_c = a;
        }

        if (line.indexOf("_cell_length_alpha",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a;
            QString temp;
            buffer >> temp >> a;
            this->_alpha = a;
        }

        if (line.indexOf("_cell_length_beta",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a;
            QString temp;
            buffer >> temp >> a;
            this->_beta = a;
        }

        if (line.indexOf("_cell_length_gamma",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a;
            QString temp;
            buffer >> temp >> a;
            this->_gama = a;
        }

        if (line.indexOf("_diffrn_radiation_wavelength",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a;
            QString temp;
            buffer >> temp >> a;
            this->_wavelength = a;
        }
    }

    file.close();

}


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
        << crfile.gama();
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
        >> crfile._gama;

    crfile._type = static_cast<FileType>(a);
    crfile._ctype = static_cast<CellType>(b);

    return in;
}
