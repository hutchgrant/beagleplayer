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

#include "syncall.h"

syncAll::syncAll()
{

}

void syncAll::dispPref(){
    prefDg.show();
    dbCon.createLocalDB();  /// create data tables for local and playlist
    pref.initDB();
}

void syncAll::setPref(){
    if(pref.initDB() != false){
    }
    else{
        dispPref();
    }
}

void syncAll::control(fileObj &Artist, fileObj &Song, fileObj &VidDir, fileObj &Video, int rwMode){
    // reinit objects
    Artist.initFile(100); Song.initFile(100); VidDir.initFile(100); Video.initFile(100);
    // set preferences
    setPref();
    if(rwMode == 2 || rwMode == 3){
        QDir usrDir = QString(getenv("HOME"));
        usrDir = QFileDialog::getExistingDirectory(this, tr("Import a directory"), QDir::currentPath());  // get folder import directory
        if(rwMode == 2){   /// import audio
           lclSync.Sync(usrDir, 0);
        }
        else if(rwMode == 3){  /// import video
            lclSync.Sync(usrDir, 1);
        }
    }
    dbCon.readDB(Artist, "SELECT * FROM lcl_songdirs");
    dbCon.readDB(Song, "SELECT * FROM lcl_songs");
    dbCon.readDB(VidDir, "SELECT * FROM lcl_viddirs");
    dbCon.readDB(Video, "SELECT * FROM lcl_videos");
}

