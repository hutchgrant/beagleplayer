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

#include "browse.h"
#include "ui_browse.h"

browse::browse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::browse)
{
    ui->setupUi(this);
    Sync(1);
    initCueID(4, 100 , 0);
    MenuMode = 0;
    songCount = 0;
    vidCount = 0;
}

browse::~browse()
{
    delete ui;
}
void browse::Sync(int type){
    sy.control(Artist,Song,VidDir, Video, type);
    updateMenu();
}

void browse::updateMenu(){

    QStringList curMenu;
    m_Model = new QStringListModel(this);
    if(MenuMode == 0)  // artists
    {
        for(int i=0; i <= Artist.getSize(); i++){
            curMenu << Artist.getName(i);
        }
    }
    else if(MenuMode == 1) // vid Dirs
    {
        for(int i=0; i <= VidDir.getSize(); i++){
            curMenu << VidDir.getName(i);
        }
    }
    m_Model->setStringList(curMenu);
    ui->MenuList->setModel(m_Model);
}

void browse::updateTitle(int selected){
    int selID = 0;
    QStringList curSong;
    t_Model = new QStringListModel(this);
    songCount = 0;
    vidCount = 0;
        if(MenuMode == 0){
            selID = Artist.getID(selected);
            initCueID(1, Song.getSize(),1);
            for(int i = 0; i<Song.getSize(); i++){
                if(Song.getPar(i) == selID){
                    curSong << Song.getName(i);
                    curSongID[songCount] = Song.getID(i);
                    songCount++;
                }
            }
           emit curListChanged(Song, curSongID);
        }
        else if(MenuMode == 1){
            selID = VidDir.getID(selected);
            initCueID(2, Video.getSize(),1);
            for(int i = 0; i<Video.getSize(); i++){
                if(Video.getPar(i) == selID){
                    curSong << Video.getName(i);
                    curVidID[vidCount] = Video.getID(i);
                    vidCount++;
                }
            }
            emit curListChanged(Video, curVidID);
        }
    t_Model->setStringList(curSong);
    ui->TrackList->setModel(t_Model);
}

/*
  *INIT CUE ID LIST
  */
void browse::initCueID(int type, int newsize, int inital)
{
    if(inital != 0){    // we're reallocating

        if(type == 1){
            delete [] curSongID;
            curSongID = new int[newsize+1];
            for(int i = 0; i<newsize; i++){
                curSongID[i] = 0;
            }
        }
        else if(type == 2){
            delete [] curVidID;
            curVidID = new int[newsize+1];
            for(int i = 0; i<newsize; i++){
                curVidID[i] = 0;
            }
        }
    }
    else{
        curSongID = new int[newsize+1];
        curVidID = new int[newsize+1];
        for(int i = 0; i<newsize; i++){
            curSongID[i] = 0;
            curVidID[i] = 0;
        }
    }
}

void browse::on_MenuList_clicked(const QModelIndex &index)
{
    emit MenuSelection(ui->MenuList->currentIndex().row());
}

void browse::on_TrackList_clicked(const QModelIndex &index)
{
    int selected = 0;
    selected = ui->TrackList->currentIndex().row();
    emit selectionChanged(selected);

    if(MenuMode == 0){
        for(int i=0; i<=songCount; i++){
            if(curSongID[selected] == Song.getID(i)){
                emit plItemChanged(Song.getName(i), Song.getPath(i), Song.getID(i), Song.getPar(i));
            }
        }
    }
    else if(MenuMode == 1){
        for(int i =0; i<=vidCount; i++){
            if(curVidID[selected]== Video.getID(i)){
                emit plItemChanged(Video.getName(i), Video.getPath(i), Video.getID(i), Video.getPar(i));
            }
        }
    }


}

void browse::on_TrackList_doubleClicked(const QModelIndex &index)
{
    emit FullSelection(ui->TrackList->currentIndex().row());
}
