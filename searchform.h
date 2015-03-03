#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>

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

private:
    Ui::SearchForm *ui;
    double _cella;
    double _cellb;
    double _cellc;
    double _cellalpha;
    double _cellbeta;
    double _cellgamma1;

    QString _dbfile;


};

#endif // SEARCHFORM_H
