/**************************************************************************
**
** This searchform.cpp file is part of SolXd software.
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

#include "searchform.h"
#include "ui_searchform.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include "crystfile.h"
#include <QDateTime>
#include <QMessageBox>
#include <QFileInfo>

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
}

SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::startSearch()
{
    if(QFileInfo(_dbfile).exists())
        doasearch();
    else {
        QMessageBox::warning(this,tr("Database file can't be located"),tr("You probably didn't create the database file. So, do this."),QMessageBox::Close);
    }

}

void SearchForm::doasearch()
{
    _results.clear();

    QFile DBFile(_dbfile);
    DBFile.open(QIODevice::ReadOnly);
    QDataStream in(&DBFile);

    emit massage("Looking for files....");
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

    QDateTime time;
    in >> time;

    if (in.status() != QDataStream::Ok) {
        qDebug() << "in is bad. Fix it";
        in.resetStatus();
    }


    bool nigglicheck(ui->checkBox_reduces->isChecked()); // Check state for "reduce cell" check box

    const double error(ui->lineEdit_error->text().toDouble());

    while (!in.atEnd()) {
        QList<bool> boolresults;
        Crystfile crfile;
        in >> crfile;

        if (crfile.getPath() == "")
            break;

        if (ui->checkBox_advanced->isChecked()) {
            if(ui->checkBox_cellA->isChecked()){
                if(crfile.findCellABC(ui->lineEdit_cellA->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
                }

            if(ui->checkBox_cellB->isChecked())
            {
                if(crfile.findCellABC(ui->lineEdit_cellB->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }

            if(ui->checkBox_cellC->isChecked())
            {
                if(crfile.findCellABC(ui->lineEdit_cellC->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }

            if(ui->checkBox_alpha->isChecked())
            {
                if(crfile.findAngleABC(ui->lineEdit_alpha->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }

            if(ui->checkBox_beta->isChecked())
            {
                if(crfile.findAngleABC(ui->lineEdit_beta->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }

            if(ui->checkBox_gamma->isChecked())
            {
                if(crfile.findAngleABC(ui->lineEdit_gamma->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }
        } else {

            if(ui->checkBox_cellA->isChecked())
            {
                if(crfile.findCellA(ui->lineEdit_cellA->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }
            if(ui->checkBox_cellB->isChecked())
            {
                if(crfile.findCellB(ui->lineEdit_cellB->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }
            if(ui->checkBox_cellC->isChecked())
            {
                if(crfile.findCellC(ui->lineEdit_cellC->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }
            if(ui->checkBox_alpha->isChecked())
            {
                if(crfile.findCellAlpha(ui->lineEdit_alpha->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }
            if(ui->checkBox_beta->isChecked())
            {
                if(crfile.findCellBeta(ui->lineEdit_beta->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }
            if(ui->checkBox_gamma->isChecked())
            {
                if(crfile.findCellGamma(ui->lineEdit_gamma->text().toDouble(),error,nigglicheck))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
            }
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

        if(ui->checkBox_latt->isChecked()){
            CellType item = static_cast<CellType>(ui->comboBox_latt->currentIndex()+1);
             // we need to adjust indexes between widget and enum
            switch (item) {
            case PRIMITIVE:
                if (crfile.findBrave(item))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
                break;
            case ICENTERED:
                if (crfile.findBrave(item))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
                break;
            case RHOMBOHIDRAL:
                if (crfile.findBrave(item))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
                break;
            case FCENTERED:
                if (crfile.findBrave(item))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
                break;
            case ACENTERED:
                if (crfile.findBrave(item))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
                break;
            case BCENTERED:
                if (crfile.findBrave(item))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
                break;
            case CCENTERED:
                if (crfile.findBrave(item))
                    boolresults.push_back(true);
                else
                    boolresults.push_back(false);
                break;
            default:
                break;
            }// end of switch
        } // end if

        if(ui->comboBox_center->currentIndex() != 0)
        {
            if((crfile.isCenter() && ui->comboBox_center->currentIndex() == 1) ||
                    (!crfile.isCenter() && ui->comboBox_center->currentIndex() == 2)    )
                boolresults.push_back(true);
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
        }
    }

    DBFile.close();

    for (int var = 0; var < _results.size(); ++var) {
        if(_results.at(var).getSfacArray().size() != _results.at(var).getUnitArray().size()){
            qDebug() << "problem with " << _results.at(var).getPath();
            qDebug() << "SFAC size = " << _results.at(var).getSfacArray().size();
            qDebug() << "UNIT size = " << _results.at(var).getUnitArray().size();
        }
    }
    emit finished(_results);

}
