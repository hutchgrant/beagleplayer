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

#include "playlist.h"
#include "ui_playlist.h"

playlist::playlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::playlist)
{
    ui->setupUi(this);
    PLMODE = 0;
    fillPL();
    plSelected = 0;
    newItemCount = 0;
}

playlist::~playlist()
{
    delete ui;
}
void playlist::createNewPL(){
    newplDg.show();
    if(newplDg.Accepted){
        newPLName = newplDg.getName();
    }
    writeNew(0);
}

void playlist::writeNew(int type){
    char *addQry;
    if(type == 0){
        addQry = new char[strlen(newPLName.c_str())+250];
        sprintf(addQry, "INSERT INTO playlists (lcl_dir_name) VALUES ('%s'", newPLName.c_str());
        dbCon.writeMe(string(addQry));
    }
    else{
        dbCon.writeDB(newPList, newItemCount, 4);
    }
    delete [] addQry;
}

void playlist::readPL(){
    dbCon.readPL(playlists, playlistItems);
}

void playlist::fillPL(){
    /// recheck db ///
    readPL();
    QStringList curList;
    pl_model = new QStringListModel(this);

    if(PLMODE == 0){  // browsing playlists
        initCueID(0, playlists.getSize());
        for(int i= 0; i<= playlists.getSize(); i++){
            curList << playlists.getName(i);
            curPLlist[i] = playlists.getID(i);
        }
    }
    else{            // browsing playlist items
        initCueID(1, playlistItems.getSize());
        for(int i = 0; i<= playlistItems.getSize(); i++){
            if(playlistItems.getPar(i) == playlists.getID(playlistSelected)){
                curList << playlistItems.getName(i);
                curPLlist[i] = playlists.getID(i);
            }
        }
        emit playlistChanged(playlistItems, curPLlist);
    }
    pl_model->setStringList(curList);
    ui->PLAYLIST->setModel(pl_model);
}
void playlist::on_PLAYLIST_doubleClicked(const QModelIndex &index)
{
    if(PLMODE == 0){
        playlistSelected = ui->PLAYLIST->currentIndex().row();
        PLMODE = 1;
        fillPL();
    }
    plSelected = ui->PLAYLIST->currentIndex().row();
    emit playlistFullSelection(plSelected);
}

void playlist::on_PLAYLIST_clicked(const QModelIndex &index)
{
    if(PLMODE == 0){
        playlistSelected = ui->PLAYLIST->currentIndex().row();
    }
    else if(PLMODE == 1){
        plSelected = ui->PLAYLIST->currentIndex().row();
        emit playlistSelection(plSelected);
    }
}

void playlist::initCueID(int initial, int newsize){

    if(initial != 0){
        delete [] curPLlist;
    }
    curPLlist = new int[newsize];
    for(int i=0; i<=newsize; i++){
        curPLlist[i] = 0;
    }
}

void playlist::on_ADD_but_clicked()
{
    if(PLMODE==0){
        createNewPL();
        AddToPL();
    }
    else if(PLMODE==1){

    }
}
