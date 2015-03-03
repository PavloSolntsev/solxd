#include "searchform.h"
#include "ui_searchform.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include "crystfile.h"

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);

    connect(ui->pushButton_start,SIGNAL(clicked()),this,SLOT(startSearch()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(close()));

}

SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::startSearch()
{
    QFile DBFile(_dbfile);
    DBFile.open(QIODevice::ReadOnly);
    QDataStream in(&DBFile);

    qDebug() << "Reading from database " << _dbfile;
    quint32 magic;
    in >> magic;
    try {
        if (magic != 0xA0B0C0D0)
            throw "Wrong file format. Database file has been broken. Reindex the database.";
        else
            qDebug() << "Magic pass is sussecful";

    } catch (const char *s) {
        qDebug() << s;
    }

    qint32 version;
    in >> version;

    try {
        if (version != 123)
            throw "Wrong version of Database file. Reindex the database";
        else
            in.setVersion(QDataStream::Qt_4_6);

    } catch (const char *s) {
        qDebug() << s;
    }

    if (!in.status()) {
        qDebug() << "in is bad. Fix it";
        in.resetStatus();
    }

    while (in.status()) {
        Crystfile crfile;
        in >> crfile;
        qDebug() << "File in database " << crfile.getPath();
    }

    DBFile.close();

}
