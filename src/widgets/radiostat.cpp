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

#include "radiostat.h"
#include "ui_radiostat.h"

/*
 *  Constructor
 */
radiostat::radiostat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::radiostat)
{
    ui->setupUi(this);
    /// init globals
    categoryCreate = false;
    rad.initFile(10);
    cat.initFile(10);
    ui->radio_cat_new->setVisible(false); /// hide new category element
}

/*
 *  Initialize Dialog cache
 */
void radiostat::init(cache *ca){
    cah = ca;
    fillCategories();
    fillRadios();
}

/*
 *  Destructor
 */
radiostat::~radiostat()
{
    delete ui;
}

/*
 *  Fill Radios in list
 */
void radiostat::fillRadios(){
    cah->readDB(rad, "radios");
    QStringList radMenu;
    r_Model = new QStringListModel(this);
    for(int i=0; i <= rad.getSize(); i++){
            radMenu << rad.getName(i);
     }
    r_Model->setStringList(radMenu);
    ui->radio_list->setModel(r_Model);
}

/*
 *  Fill Categories in spinner
 */
void radiostat::fillCategories(){
    // clear cat
    ui->radio_cat_in->clear();
    cat.initFile(10);
    // read db
    cah->readDB(cat, "categories");
    // fill
    for(int i=0; i< cat.getSize(); i++){
        ui->radio_cat_in->addItem(cat.getName(i));
    }
}

/*
 *  Radio Dialog Add/Save Category Button (modify dialog layout)
 */
void radiostat::on_radio_cat_add_clicked()
{
    if(!categoryCreate){
        categoryCreate = true;
        ui->radio_cat_new->setVisible(true);
        ui->radio_cat_add->setText("save");
    }else{
        fileObj catCreate;
        catCreate.set(0,0,0,ui->radio_cat_new->text().toStdString().c_str());
        cah->writeDB(&catCreate, "categories");
         /// reset dialog layout
        categoryCreate = false;
        ui->radio_cat_new->setVisible(false);
        ui->radio_cat_add->setText("add");
        fillCategories();
    }
}

/*
 *  Radio Dialog Menu List Selected (display edit options)
 */
void radiostat::on_radio_list_clicked(const QModelIndex &index)
{
    int select = 0;
    select = ui->radio_list->currentIndex().row();    /// get Selected Radio

    /// determine parent ID of the selected radio
    /// determine the ID of the selected category
    for(int i =0; i< cat.getSize(); i++){
        if(cat.getID(i) ==  rad.getPar(select)){
            ui->radio_cat_in->setCurrentIndex(i);   /// set category to index
        }
    }
    ui->radio_name_in->setText(rad.getName(select)); /// set radio Name
    ui->radio_url_in->setText(rad.getPath(select));  /// set radio URL
}

/*
 *  Radio Dialog Save Radio Station Button Clicked
 */
void radiostat::on_radio_save_clicked()
{
    string stationName = "", stationURL = "", stationCat = "";
    int catID = 0;

    stationName = ui->radio_name_in->text().toStdString();
    stationURL = ui->radio_url_in->text().toStdString();
    stationCat = ui->radio_cat_in->currentText().toStdString();

    /// determine the ID of the selected category
    for(int i =0; i< cat.getSize(); i++){
        if(cat.getName(i) == stationCat){
            catID = cat.getID(i);
        }
    }
    /// write new radio to database
    fileObj radCreate;
    radCreate.set(0, 0, catID, stationName.c_str(), stationURL.c_str());

    cah->writeDB(&radCreate, "radios");
    fillRadios();
}

/*
 *  Radio Dialog New Radio Station Button Clicked
 */
void radiostat::on_radio_create_clicked()
{
    ui->radio_name_in->setText("");
    ui->radio_url_in->setText("");
}

/*
 *  Radio Dialog Category Remove Click
 */
void radiostat::on_radio_cat_rem_clicked()
{
    int select = 0;
    select = ui->radio_cat_in->currentIndex();
    cah->removeFrom(cat.getID(select), "categories", false);
    fillCategories();
}
