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
