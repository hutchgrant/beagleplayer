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

/* Playlist widget
 *
 * PLMODE = 0  - Browse playlists by name
 * PLMODE = 1  - Browse playlists by items
 * PLMODE = 2  - Browse playlists by displaying only a newly created playlist's items
 */

/*
 *  Constructor
 */
playlist::playlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::playlist)
{
    plName = "";
    PlayMode = 0;
    tempPar = 0;
    tempPath = "";
    tempTrack = "";
    PLMODE = 0;
    pListSelect = 0;
    pItemSelect = 0;

    ui->setupUi(this);
}

/*
 *  Initializor
 */
void playlist::init(){
    pList.initFile(100);
    pListItems.initFile(100);
    curPLlist = new int[pListItems.getSize() + 10];
}

/*
 *  Destructor
 */
playlist::~playlist()
{
    delete ui;
}

/*
 *  sync and initialize all playlists and playlist items from cache
 */
bool playlist::readPL(){
   init();
   dbCon->readDB(pList, "playlists");
   dbCon->readDB(pListItems, "playlist_items");
   return true;
}

/*
 *  Fill Playlist Right Window
 */
void playlist::fillPL(){
    QStringList curList;
    int count = 0;
    if(PLMODE == 0){  // browsing playlists
        for(int i= 0; i< pList.getSize(); i++){
            curList << pList.getName(i);
        }
    }
    else if(PLMODE == 1){            // browsing playlist items
        curPLlist = new int[pListItems.getSize()];
        for(int i = 0; i< pListItems.getSize(); i++){
            if(pListItems.getPar(i) == pList.getID(pListSelect)){
                curList << pListItems.getName(i);
                curPLlist[count] = pListItems.getID(i);
                count++;
            }
        }
        emit playlistChanged(pListItems, curPLlist, count, false, PlayMode);
    }
    else if(PLMODE == 2){            // new playlist items
        curPLlist = new int[pNewItems.getSize()];
        for(int i = 0; i< pNewItems.getSize(); i++){
            curList << pNewItems.getName(i);
            curPLlist[count] = pNewItems.getID(i);
            count++;
        }
        emit playlistChanged(pNewItems, curPLlist, count, false, PlayMode);
    }
    pl_model = new QStringListModel(this);
    pl_model->setStringList(curList);
    ui->PLAYLIST->setModel(pl_model);
}

/*
 *  Display new playlist dialog, write playlist name to playlists table
 */
void playlist::createNewPL(){

    pNewList = fileObj();
    pNewItems = fileObj();
    pNewList.initFile(10);
    pNewItems.initFile(20);

    plDialog.show();
    if(plDialog.exec()==QDialog::Accepted){
        pNewList.set(pNewList.getSize(), 0, 0, plDialog.getName().c_str());
        pNewList.setID(pNewList.getSize(), dbCon->writeDB(&pNewList, "playlists"));
    }
}

/*
 * Add to new playlist object, insert into playlist_items table
 */
void playlist::addToNewPL(string track, string path){
    pNewItems = fileObj();
    pNewItems.initFile(10);
    pNewItems.set(pNewItems.getSize(), 0, pNewList.getID(pNewList.getSize()), track.c_str(), path.c_str());
    dbCon->writeDB(&pNewItems, "playlist_items");
}

/*
 * Add to a current playlist
 */
void playlist::addToCurrent(int parid, string track, string path){
    pNewList = fileObj();
    pNewList.initFile(10);
    pNewList.set(0, 0, parid, track.c_str(), path.c_str());
    dbCon->writeDB(&pNewList, "playlist_items");
}

/*
 *  Playlist item double clicked, grab the index, setup the window/controls
 */
void playlist::on_PLAYLIST_doubleClicked(const QModelIndex &index)
{
    int selected = 0;
    selected = ui->PLAYLIST->currentIndex().row();
    tempPar = pList.getPar(selected);
    tempID = pList.getID(selected);
    tempPath = pList.getPath(selected);
    tempTrack = pList.getName(selected);
    if(PLMODE == 0){
        pListSelect = selected;
        PLMODE = 1;
        fillPL();
    }
    else{
        pItemSelect = selected;
        emit playlistFullSelection(pItemSelect);
    }
}

/*
 *  When any item is clicked in the playlist window, grab the index, setup window/controls
 */
void playlist::on_PLAYLIST_clicked(const QModelIndex &index)
{
    int selected = 0;
    selected = ui->PLAYLIST->currentIndex().row();

    tempPar = pList.getPar(selected);
    tempID = pList.getID(selected);
    tempPath = pList.getPath(selected);
    tempTrack = pList.getName(selected);
    if(PLMODE == 0){
        pListSelect = selected;
    }
    else if(PLMODE == 1){
        pItemSelect = selected;
        emit playlistSelection(pItemSelect);
    }
}

/*
 *  browse/open playlists
 */
void playlist::on_open_tool_clicked()
{
    PLMODE = 0;
    if(readPL()){
       fillPL();
    }
}

/*
 *  When playlist button " + " is clicked, add to playlist, if none open, create a new one, else add to playlist
 */
void playlist::on_add_tool_clicked()
{
    if(PLMODE==0){
        createNewPL();
        addToNewPL(tempTrack, tempPath);
        PLMODE = 1;
        pListSelect = pList.getSize();

        if(readPL()){
            fillPL();
        }
    }
    else if(PLMODE==1){
        addToNewPL(tempTrack, tempPath);
        if(readPL()){
            fillPL();
        }
    }else if(PLMODE ==2){  // add to current playlst
        addToCurrent(tempPar, tempTrack, tempPath);
        if(readPL()){
            fillPL();
        }
    }
}

/*
 *  When playlist remove button " - " is clicked, remove folder/files/file
 */
void playlist::on_remove_tool_clicked()
{
    int selected = 0;
    selected = ui->PLAYLIST->currentIndex().row();

    if(PLMODE == 0){
        dbCon->removeFrom(pList.getID(selected), "playlists", false);
        for(int x=0; x< pListItems.getSize(); x++){
            if(pListItems.getPar(x) == pList.getID(selected)){
                dbCon->removeFrom(pListItems.getPar(selected), "playlist_items", true);
            }
        }
    }else{
        dbCon->removeFrom( curPLlist[selected], "playlist_items", false);
    }
    if(readPL()){
        fillPL();
    }
}
