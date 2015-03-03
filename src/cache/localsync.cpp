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
    int FolderCount = 0;
    string firstTable = "", secondTable = "";
    QString curDir = NULL;
    dbCon.init();

    if(usrDir != QString(getenv("HOME"))){
        curDir = usrDir.absolutePath();  // get chosen path
        scanDir(curDir, syncType);
        //sync to db
        if(syncType == 0){  /// sync Audio
            firstTable = "songdirs";
            secondTable = "songs";
        }else{
            firstTable = "viddirs";
            secondTable = "videos";
        }
         // scan main for directories
         dbCon.writeDB(&localDir, firstTable);  // write song directories
         FolderCount = localDir.getSize();
         /// reinit localDirectorie names, paths, ids,
         localDir = fileObj();
         localDir.initFile(100);
         dbCon.readDB(localDir, firstTable);  // re-read song directories with their new key ids
         /// Scan + write files from each directory
         scanFiles(syncType, FolderCount);
         dbCon.writeDB(&localFile, secondTable);  // write song files
    }
}
/*
* Scan User Directory for folders. Add all discovered folders
*/
void localsync::scanDir(QString dir, int scantype){
    QDirIterator directories(dir, QDir::Dirs | QDir::NoDotAndDotDot);
    int count = 0;
    string fileName = "", filePath = "";

    localDir = fileObj();
    localDir.initFile(100);

    fileName = sanitizeName(directories.fileName());
    filePath = sanitizeName(directories.filePath());
    localDir.set(count, 0,  0, fileName.c_str(), filePath.c_str());
    count++;

    while(directories.hasNext()){
        directories.next();
        fileName = sanitizeName(directories.fileName());
        filePath = sanitizeName(directories.filePath());
        localDir.set(count, 0,  0, fileName.c_str(), filePath.c_str());
        count++;

        QDirIterator subdirect(directories.filePath(), QDir::Dirs | QDir::NoDotAndDotDot);
        while(subdirect.hasNext()){
            subdirect.next();
            fileName = sanitizeName(subdirect.fileName());
            filePath = sanitizeName(subdirect.filePath());
            localDir.set(count, 0,  0, fileName.c_str(), filePath.c_str());
            count++;
        }

    }
}
/*
* Scan User Directory for folders. Add all discovered files
*/
void localsync::scanFiles(int scanType, int folderCount){
    localDir.display();
    int itemcount = 0;
    string fileName = "", filePath = "";

    localFile = fileObj();
    localFile.initFile(100);

    for(int i=localDir.getSize(); i>localDir.getSize()-folderCount-1; i--){
        QDirIterator dirWalk(QString::fromStdString(localDir.getPath(i)), QDir::Files | QDir::NoSymLinks);
        while(dirWalk.hasNext())
        {
            dirWalk.next();
            if(scanType == 0){  // if scanning audio
                if(dirWalk.fileInfo().completeSuffix() == "mp3" || dirWalk.fileInfo().completeSuffix() == "flac" || dirWalk.fileInfo().completeSuffix() == "wav"){
                    fileName = sanitizeName(dirWalk.fileName());
                    filePath = sanitizeName(dirWalk.filePath());
                    localFile.set(itemcount,itemcount, localDir.getID(i), fileName.c_str(), filePath.c_str());
                    itemcount++;
                }
            }
            else{   // if scanning video
                if(dirWalk.fileInfo().suffix() == "avi" || dirWalk.fileInfo().suffix() == "mp4" || dirWalk.fileInfo().suffix() == "mkv"){
                    fileName = sanitizeName(dirWalk.fileName());
                    filePath = sanitizeName(dirWalk.filePath());
                    localFile.set(itemcount,itemcount, localDir.getID(i),fileName.c_str(), filePath.c_str());
                    itemcount++;
                }
            }
        }
    }
}

string localsync::sanitizeName(QString filename){
    filename.replace(QString("'"), QString("$_"));
    return filename.toStdString().c_str();
}

localsync::~localsync(){

}
