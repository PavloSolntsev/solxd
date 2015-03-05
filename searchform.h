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
    void setDBfile(const QString &a){_dbfile = a;}

    ~SearchForm();

public slots:
    void startSearch();
//    void setA(const double &a){_cella = a;}
//    void setB(const double &a){_cellb = a;}
//    void setC(const double &a){_cellc = a;}
//    void setAlpha(const double &a){_cellalpha = a;}
//    void setBeta(const double &a){_cellbeta = a;}
//    void setGama(const double &a){_cellgamma = a;}

signals:
    void finished(const QList<Crystfile> &);

private:
    Ui::SearchForm *ui;
//    double _cella;
//    double _cellb;
//    double _cellc;
//    double _cellalpha;
//    double _cellbeta;
//    double _cellgamma;

    QString _dbfile;
    QList<Crystfile> _results;


};

#endif // SEARCHFORM_H
