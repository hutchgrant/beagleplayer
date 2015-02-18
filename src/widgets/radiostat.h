#ifndef RADIOSTAT_H
#define RADIOSTAT_H

#include <QDialog>
#include "./src/cache/cache.h"

namespace Ui {
class radiostat;
}

class radiostat : public QDialog
{
    Q_OBJECT

public:
    fileObj rad, cat;
    bool categoryCreate;

    explicit radiostat(QWidget *parent = 0);
    void init(cache *ch);
    void fillRadios();
    void fillCategories();
    virtual ~radiostat();

private slots:
    void on_radio_list_clicked(const QModelIndex &index);

    void on_radio_cat_add_clicked();

    void on_radio_save_clicked();

    void on_radio_create_clicked();

    void on_radio_cat_rem_clicked();

private:
    Ui::radiostat *ui;
    cache *cah;
    QStringListModel *r_Model;
};

#endif // RADIOSTAT_H
