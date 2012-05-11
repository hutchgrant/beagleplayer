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

#include "localsync.h"

localsync::localsync()
{
    AudFolderCount = 0;
    VidFolderCount = 0;
    AudioCount = 0;
    VideoCount = 0;
}

/*
  * When Add is clicked
  */
void localsync::Sync(QDir usrDir, int syncType)
{
    QString curDir = NULL;

    if(usrDir != QString(getenv("HOME"))){
        curDir = usrDir.absolutePath();  // get chosen path
        dbCon.getLastIDs(&AudFolderCount, &VidFolderCount, &AudioCount, &VideoCount);
        fileObj localDir, localFile;
        scanDir(curDir, syncType, localDir);

      //  scanFiles(syncType);
        //sync to db
        if(syncType == 0){  /// sync Audio
            // scan main for directories
            dbCon.writeDB(localDir, AudFolderCount, 0);  // write song directories
            scanFiles(localFile, syncType);
            dbCon.writeDB(localFile, AudioCount, 1);  // write song files
        }
        else{  /// sync video
            dbCon.writeDB(localDir, VidFolderCount, 2);  // write song directories
            scanFiles(localFile, syncType);
            dbCon.writeDB(localFile, VideoCount, 3);  // write song file
        }
    }
}

/*
* Scan User Directory for folders. Add all discovered folders
*/
void localsync::scanDir(QString dir, int scantype, fileObj &localDir){
    QDirIterator directories(dir, QDir::Dirs | QDir::NoDotAndDotDot);
    int count = 0;
    localDir.initFile(100);
    while(directories.hasNext()){

        directories.next();
        if(scantype == 0 ){
            localDir.set(count, AudFolderCount, 0, directories.fileName().toStdString().c_str(), directories.filePath().toStdString().c_str());
            AudFolderCount++;
        }
        else{
            localDir.set(count, VidFolderCount, 0, directories.fileName().toStdString().c_str(), directories.filePath().toStdString().c_str());
            VidFolderCount++;

        }
        count++;
    }
    localDir.display();
}

/*
* Scan User Directory for folders. Add all discovered files
*/
void localsync::scanFiles(fileObj &localFile,  int scanType){

    int itemcount = 0;
    int countSize = 0;
    fileObj playObj;

    if(scanType == 0){
        countSize = AudFolderCount;
        dbCon.readDB(playObj, "SELECT * FROM lcl_songdirs");

    }
    else{
        countSize = VidFolderCount;
        dbCon.readDB(playObj, "SELECT * FROM lcl_viddirs");

    }
    for(int i=0; i<countSize; i++){

        QDirIterator dirWalk(QString::fromStdString(playObj.getPath(i)), QDir::Files | QDir::NoSymLinks);

        while(dirWalk.hasNext())
        {
            dirWalk.next();
            if(scanType == 0){  // if scanning audio
                if(dirWalk.fileInfo().completeSuffix() == "mp3" || dirWalk.fileInfo().completeSuffix() == "flac"){
                    localFile.set(itemcount,VideoCount, playObj.getID(i), dirWalk.fileName().toStdString().c_str(), dirWalk.filePath().toStdString().c_str());
                    AudioCount++;
                    itemcount++;
                }
            }
            else{   // if scanning video

                VidFolderCount++;
                if(dirWalk.fileInfo().suffix() == "avi" || dirWalk.fileInfo().suffix() == "mp4" || dirWalk.fileInfo().suffix() == "mkv"){
                    localFile.set(itemcount,VideoCount, playObj.getID(i), dirWalk.fileName().toStdString().c_str(), dirWalk.filePath().toStdString().c_str());
                    VideoCount++;
                    itemcount++;
                }
            }
        }
    }
    localFile.display();
}

localsync::~localsync(){

}
