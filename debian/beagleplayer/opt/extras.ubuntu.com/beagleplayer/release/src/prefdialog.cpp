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


#include "prefdialog.h"
#include "ui_prefdialog.h"
using namespace std;

PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefDialog)
{
    ui->setupUi(this);
    setLabels();
}

void PrefDialog::setLabels(){
    ui->entry_sql->setText(QString(TEMPDB));
}

PrefDialog::~PrefDialog()
{
    delete ui;
}
void PrefDialog::on_buttonBox_accepted()
{
    setPreferences();
}
void PrefDialog::setPreferences(){

    QString Q_sql = ui->entry_sql->text();
        setSQL(Q_sql.toStdString());

}
