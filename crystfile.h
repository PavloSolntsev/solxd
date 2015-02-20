#ifndef CRYSTFILE_H
#define CRYSTFILE_H
#include "unitcell.h"
#include <QString>

class Crystfile : public Unitcell
{
public:
    Crystfile();
    Crystfile(FileType type, const QString &path);
//    void setPath(const QString &path){_path=path;}
//    void setPath(const char *path){_path=path;}
    const QString &getPath()const {return _path;}
private:
    QString _path;
    FileType _type;

    void parseINS();
    void parseCIF();
};

#endif // CRYSTFILE_H
