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

/*
 * Constructor
 */
browse::browse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::browse)
{
    ui->setupUi(this);
    MenuMode = 0;
    songCount = 0;
    vidCount = 0;
    radCount = 0;
}

/*
 * Destructor
 */
browse::~browse()
{
    delete ui;
}

/*
 *  Initialize/update/re-read from cache.
 */
void browse::Sync(int type){
    if(type == 2 || type == 3){    /// import new entries into local db for each folder and file
        QDir usrDir = QString(getenv("HOME"));
        QFileDialog iDirect;

        usrDir = iDirect.getExistingDirectory(this, tr("Import a directory"), usrDir.absolutePath());
        if(usrDir.dirName() != NULL || usrDir.dirName() != ""){
            if(type == 2){   /// import audio
               if(lclSync.Sync(usrDir, 0)){
                   MenuMode = 0;
                }
            }
            else if(type == 3){  /// import video
                if(lclSync.Sync(usrDir, 1)){
                    MenuMode = 1;
                }
            }
        }
    }else if(type == 4){
        radStat = new radiostat();
        radStat->init(dbCon);
        radStat->show();
        MenuMode = 2;  /// set Mode to radio
        updateTitle(0);
    }else if(type == 0){
        QDir usrDir;
         usrDir = QFileDialog::getOpenFileName(this, tr("Open a Audio/Video file"), QDir::currentPath(), tr("Video/Audio (*.avi *.mp4 *.mp3 *.flac *.wav)"));
         if(usrDir.dirName() != NULL || usrDir.dirName() != ""){
            emit startTempTrack(usrDir.dirName().toStdString(), usrDir.path().toStdString(), MenuMode);
         }
    }else if(type == -1){
        web = new QWebUrl();
        web->show();
        if(web->exec() == QDialog::Accepted){
            if(web->url_input != "http://" || web->url_input != ""){
                emit startTempTrack("", web->url_input, MenuMode);
            }
        }
    }

    if(type ==1 || type == 2 || type == 3){   /// don't sync if we're playing a temp file
        syncCache();
        updateMenu();
    }
}

/*
 * Sync all Objects from cache
 */
void browse::syncCache(){
    Artist.initFile(100); Song.initFile(100); VidDir.initFile(100); Video.initFile(100), Radio.initFile(10), RadioCat.initFile(10);
    dbCon->readAll(Artist, Song, VidDir, Video);/// read from local database and sync to local objects
    dbCon->readDB(Radio, "radios");
    dbCon->readDB(RadioCat, "categories");
}

/*
 *  Update Left View List with Media file type, and their paths
 */
void browse::updateMenu(){
    QStringList curMenu = QStringList();
    m_Model = new QStringListModel(this);
    if(MenuMode == 0 )  // artists + local
    {
        for(int i=0; i < Artist.getSize(); i++){
            curMenu << Artist.getName(i);
        }
    }
    else if(MenuMode == 1 ) // vid Dirs + local
    {
        for(int i=0; i < VidDir.getSize(); i++){
            curMenu << VidDir.getName(i);
        }
    }
    else if(MenuMode == 2){ // radio stations + reminder: use categories later
        for(int i=0; i < RadioCat.getSize(); i++){
            curMenu << RadioCat.getName(i);
        }
    }
    m_Model->setStringList(curMenu);
    ui->MenuList->setModel(m_Model);
}

/*
 *  Update Right View List with Media file tracks for a specific path
 */
void browse::updateTitle(int selected){
    int selID = 0;
    QStringList curSong;
    t_Model = new QStringListModel(this);
    songCount = 0;
    vidCount = 0;
    radCount = 0;
        if(MenuMode == 0 ){    /// set Song + local
            selID = Artist.getID(selected);
            curSongID = new int[Song.getSize()];
            for(int i = 0; i<Song.getSize(); i++){
                if(Song.getPar(i) == selID){
                    curSong << Song.getName(i);
                    curSongID[songCount] = Song.getID(i);
                    songCount++;
                }
            }
           emit curListChanged(selID, &Artist, &Song, curSongID, songCount, false, MenuMode);
        }
        else if(MenuMode == 1 ){    ///  set Video + local
            selID = VidDir.getID(selected);
            curVidID = new int[Video.getSize()];
            for(int i = 0; i<Video.getSize(); i++){
                if(Video.getPar(i) == selID){
                    curSong << Video.getName(i);
                    curVidID[vidCount] = Video.getID(i);
                    vidCount++;
                }
            }
            emit curListChanged(selID, &VidDir, &Video, curVidID, vidCount, false, MenuMode);
        }
        else if(MenuMode == 2 ){    ///  set Radio
            selID = RadioCat.getID(selected);
            curRadID = new int[Radio.getSize()];
            for(int i = 0; i<Radio.getSize(); i++){
                if(Radio.getPar(i) == selID){
                    curSong << Radio.getName(i);
                    curRadID[radCount] = Radio.getID(i);
                    radCount++;
                }
            }
            emit curListChanged(selID, &RadioCat, &Radio, curRadID, radCount, true, MenuMode);
        }
    t_Model->setStringList(curSong);
    ui->TrackList->setModel(t_Model);
}

/*
 *  When a Directory/Folder on the left hand browse menu is selected
 */
void browse::on_MenuList_clicked(const QModelIndex &index)
{
    updateTitle(ui->MenuList->currentIndex().row());
}

/*
 *  When a Track/Video/file on the right hand browse menu is selected(once)
 */
void browse::on_TrackList_clicked(const QModelIndex &index)
{
    int selected = 0;
    selected = ui->TrackList->currentIndex().row();
    emit selectionChanged(selected);
    /// submit new clicked listing
    if(MenuMode == 0){             /// listing local songs
        for(int i=0; i<=Song.getSize(); i++){
            if(curSongID[selected] == Song.getID(i)){
                emit trackChanged(Song.getName(i), Song.getPath(i), Song.getID(i), Song.getPar(i), MenuMode);
            }
        }
    }
    else if(MenuMode == 1){         /// listing local videos
        for(int i =0; i<=vidCount; i++){
            if(curVidID[selected]== Video.getID(i)){
                emit trackChanged(Video.getName(i), Video.getPath(i), Video.getID(i), Video.getPar(i), MenuMode);
            }
        }
    }  
    else if(MenuMode == 2){         /// listing local videos
        for(int i =0; i<=radCount; i++){
            if(curRadID[selected]== Radio.getID(i)){
                emit trackChanged(Radio.getName(i), Radio.getPath(i), Radio.getID(i), Radio.getPar(i), MenuMode);
            }
        }
    }
 }

/*
 *  When a Track/Video/file on the right hand browse menu is doubleClicked
 */
void browse::on_TrackList_doubleClicked(const QModelIndex &index)
{
    emit FullSelection(ui->TrackList->currentIndex().row());
}

