#include "crystfile.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>


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

bool Crystfile::findWL(const double &wl)
{
    if(abs(_wavelength - wl) < 0.001)
        return true;
    else
        return false;

}

bool Crystfile::findSfac(const QString &sfac)
{
    if(sfacarray.contains(sfac))
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

        if (line.indexOf("cell",0,Qt::CaseInsensitive) == 0)
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

        if (line.indexOf("latt",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            int a;
            QString temp;
            buffer >> temp >> a;

            if (a>0) {
                _center = true;
            }

            a = abs(a);

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

        if (line.indexOf("sfac",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            QString a;
            QString temp;

            buffer >> temp;

            while (buffer.status()) {
                buffer >> a;
                sfacarray.push_back(a.toLower());
            }

            sfaccheck = true;
            continue;
        }

        if (line.indexOf("unit",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a;
            QString temp;

            buffer >> temp;

            while (buffer.status()) {
                buffer >> a;
                unitarray.push_back(a);
            }

            unitcheck = true;

            int index = unitarray.indexOf(0);
            while(index > 0)
            {
                unitarray.removeAt(index);
                sfacarray.removeAt(index);
                index = unitarray.indexOf(0);
            }

            continue;
        }

        if(cellcheck && lattcheck && sfaccheck && unitcheck)
            break;
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

QDataStream &operator <<(QDataStream &out, const Crystfile &crfile)
{
    out << crfile._path
        << (int)crfile._type
        << crfile._ctype
        << crfile._wavelength
        << crfile._center
        << crfile.sfacarray
        << crfile.unitarray;
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
        >> crfile.unitarray;

    crfile._type = static_cast<FileType>(a);
    crfile._ctype = static_cast<CellType>(b);

    return in;
}
