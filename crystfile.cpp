#include "crystfile.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>


Crystfile::Crystfile():_type(NONE),_path("")
{



}


Crystfile::Crystfile(FileType type, const QString &path):
    _type(type),
    _path(path)
{
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

void Crystfile::parseINS()
{
    QFile file(_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream inp(&file);

    while(!inp.atEnd()) {
        QString line = inp.readLine();

        if (line.indexOf("cell",0,Qt::CaseInsensitive) == 0)
        {
            QTextStream buffer(&line);

            double a,b,c,d,e,f;
            QString temp;
            buffer >> temp >> temp >> a >> b >> c >> d >> e >> f;
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
    }

    file.close();

}
