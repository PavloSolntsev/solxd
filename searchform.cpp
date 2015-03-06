#include "searchform.h"
#include "ui_searchform.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include "crystfile.h"

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm)
//    _cella(0),
//    _cellb(0),
//    _cellc(0),
//    _cellalpha(0),
//    _cellbeta(0),
//    _cellgamma(0)
{
    ui->setupUi(this);

    connect(ui->pushButton_start,SIGNAL(clicked()),this,SLOT(startSearch()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(hide()));

}

SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::startSearch()
{
    _results.clear();
    QFile DBFile(_dbfile);
    DBFile.open(QIODevice::ReadOnly);
    QDataStream in(&DBFile);

    qDebug() << "Reading from database " << _dbfile;
    quint32 magic;
    quint16 version;
    in >> magic >> version;
    try {
        if (magic != quint32(0x12345678))
            throw "Wrong file format. Database file has been broken. Reindex the database.";
        else
        {
            if (version > in.version())
                throw "File is from a more recent version of the application";
            else
                qDebug() << "Version was passed sussecfully";
        }
    } catch (const char *s) {
        qDebug() << s;
    }

    in.setVersion(version);

    if (in.status() != QDataStream::Ok) {
        qDebug() << "in is bad. Fix it";
        in.resetStatus();
    }

    const double error(ui->lineEdit_error->text().toDouble());

    while (!in.atEnd()) {
        QList<bool> boolresults;
        Crystfile crfile;
        in >> crfile;
//        qDebug() << "File in database " << crfile.getPath();
        if (crfile.getPath() == "")
            break;

        if(ui->checkBox_cellA->isChecked())
        {
            if(crfile.findCellA(ui->lineEdit_cellA->text().toDouble(),error))
                boolresults.push_back(true);
            else
                boolresults.push_back(false);
        }
        if(ui->checkBox_cellB->isChecked())
        {
            if(crfile.findCellB(ui->lineEdit_cellB->text().toDouble(),error))
                boolresults.push_back(true);
            else
                boolresults.push_back(false);
        }
        if(ui->checkBox_cellC->isChecked())
        {
            if(crfile.findCellC(ui->lineEdit_cellC->text().toDouble(),error))
                boolresults.push_back(true);
            else
                boolresults.push_back(false);
        }
        if(ui->checkBox_alpha->isChecked())
        {
            if(crfile.findCellAlpha(ui->lineEdit_alpha->text().toDouble(),error))
                boolresults.push_back(true);
            else
                boolresults.push_back(false);
        }
        if(ui->checkBox_beta->isChecked())
        {
            if(crfile.findCellBeta(ui->lineEdit_beta->text().toDouble(),error))
                boolresults.push_back(true);
            else
                boolresults.push_back(false);
        }
        if(ui->checkBox_gamma->isChecked())
        {
            if(crfile.findCellGamma(ui->lineEdit_gamma->text().toDouble(),error))
                boolresults.push_back(true);
            else
                boolresults.push_back(false);
        }
        if (ui->checkBox_wl->isChecked())
        {
            if (crfile.findWL(ui->lineEdit_wl->text().toDouble(),error))
                boolresults.push_back(true);
            else
                boolresults.push_back(false);
        }
        if (ui->checkBox_elements->isChecked())
        {
//            for (int var = 0; var < crfile.getSfacArray().size(); ++var) {
//                qDebug() << "SFAC " <<
//                            crfile.getSfacArray().at(var) << " ";
//            }
            QString line(ui->lineEdit_elements->text());
            QTextStream sfacin(&line,QIODevice::ReadOnly);
            QString buffer;
            QList<bool> localcheck;
            while(!sfacin.atEnd())
            {
                sfacin >> buffer;
                if (!buffer.isSimpleText()) {
                    continue;
                }
//                qDebug() << "Checking for " << buffer << " elements";

                if (crfile.findSfac(buffer))
                    localcheck.push_back(true);
                else
                    localcheck.push_back(false);
            }

            if (!localcheck.contains(false)) {
                boolresults.push_back(true);
            }
            else
                boolresults.push_back(false);

        }
        if (!boolresults.contains(false))
        {
            _results.push_back(crfile);
//            qDebug() << "Found results: \n" << crfile.getPath();
//            qDebug() << crfile.a();
//            qDebug() << crfile.b();
//            qDebug() << crfile.c();
//            qDebug() << crfile.alpha();
//            qDebug() << crfile.beta();
//            qDebug() << crfile.gama();
//            qDebug() << crfile.getWavelength();

              for (int var = 0; var < crfile.getSfacArray().size(); ++var) {
                  qDebug() << "SFAC " <<
                      crfile.getSfacArray().at(var) << " ";
                }
        }
    }

    DBFile.close();

    emit finished(_results);

}
