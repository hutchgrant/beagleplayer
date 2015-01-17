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
}
/*
  * When Add is clicked
  */
void localsync::Sync(QDir usrDir, int syncType)
{
    QString curDir = NULL;
    dbCon.init();

    if(usrDir != QString(getenv("HOME"))){
        curDir = usrDir.absolutePath();  // get chosen path
        scanDir(curDir, syncType);
        //sync to db
        if(syncType == 0){  /// sync Audio
            // scan main for directories
            dbCon.writeDB(&localDir, "songdirs");  // write song directories
            dbCon.readDB(localDir, "songdirs");  // re-read song directories with their new key ids
            scanFiles(syncType);
            dbCon.writeDB(&localFile, "songs");  // write song files
        }
        else{  /// sync video
            dbCon.writeDB(&localDir, "viddirs");  // write video directories
            dbCon.readDB(localDir, "viddirs");  // re-read video directories with their new key ids
            scanFiles(syncType);
            dbCon.writeDB(&localFile, "videos");  // write song file
        }
    }
}
/*
* Scan User Directory for folders. Add all discovered folders
*/
void localsync::scanDir(QString dir, int scantype){
    QDirIterator directories(dir, QDir::Dirs | QDir::NoDotAndDotDot);
    int count = 0;
    localDir.initFile(100);
    while(directories.hasNext()){
        directories.next();

        if(scantype == 0 ){
            localDir.set(count, 0,  0, directories.fileName().toStdString().c_str(), directories.filePath().toStdString().c_str());
            count++;
        }
        else{
            localDir.set(count, 0, 0, directories.fileName().toStdString().c_str(), directories.filePath().toStdString().c_str());
            count++;
        }
    }
}
/*
* Scan User Directory for folders. Add all discovered files
*/
void localsync::scanFiles(int scanType){

    int itemcount = 0;
    localFile.initFile(100);

    for(int i=0; i<localDir.getSize(); i++){
        QDirIterator dirWalk(QString::fromStdString(localDir.getPath(i)), QDir::Files | QDir::NoSymLinks);
        while(dirWalk.hasNext())
        {
            dirWalk.next();
            if(scanType == 0){  // if scanning audio
                if(dirWalk.fileInfo().completeSuffix() == "mp3" || dirWalk.fileInfo().completeSuffix() == "flac" || dirWalk.fileInfo().completeSuffix() == "wav"){
                    localFile.set(itemcount,itemcount, localDir.getID(i), dirWalk.fileName().toStdString().c_str(), dirWalk.filePath().toStdString().c_str());
                    itemcount++;
                }
            }
            else{   // if scanning video
                if(dirWalk.fileInfo().suffix() == "avi" || dirWalk.fileInfo().suffix() == "mp4" || dirWalk.fileInfo().suffix() == "mkv"){
                    localFile.set(itemcount,itemcount, localDir.getID(i), dirWalk.fileName().toStdString().c_str(), dirWalk.filePath().toStdString().c_str());
                    itemcount++;
                }
            }
        }
    }
}

localsync::~localsync(){

}
