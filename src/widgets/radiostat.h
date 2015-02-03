/*
 *  Written By: Grant Hutchinson
 *  License: GPLv3.
 *  h.g.utchinson@gmail.com
 *  Copyright (C) 2012 by Grant Hutchinson
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
