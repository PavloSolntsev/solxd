/**************************************************************************
**
** This searchform.h file is part of SolXd software.
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

#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>
#include <QList>
#include "crystfile.h"


namespace Ui {
class SearchForm;
}

class SearchForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit SearchForm(QWidget *parent = 0);
    inline void setDBfile(const QString &a){_dbfile = a;}

    ~SearchForm();

public slots:
    void startSearch();

signals:
    void finished(const QList<Crystfile> &);
    void massage(const QString &);

private:
    Ui::SearchForm *ui;

    QString _dbfile;
    QList<Crystfile> _results;


};

#endif // SEARCHFORM_H
