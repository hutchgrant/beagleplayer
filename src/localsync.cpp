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
    initLocalObj();
}

/*
  *  Initialize safe state
  */
void localsync::initLocalObj(){
    parentID = 0;
    lastID = 0;
    AudFolderCount = 0;
    VidFolderCount = 0;
    AudioCount = 0;
    VideoCount = 0;
    AudItemCount = 0;
    AudDirCount = 0;
    VidItemCount = 0;
    VidDirCount = 0;

    AudItemINIT = 5;
    VidItemINIT = 5;
    AudDirINIT = 5;
    VidDirINIT = 5;

    initDirs(0,5, 5);
    initFiles(0, 5, 5);
}


/*
  * When Add is clicked
  */
void localsync::Sync(QDir usrDir, int syncType)
{
    //  QDir usrDir = QFileDialog::getExistingDirectory(this, tr("Import a directory"), QDir::currentPath());  // get folder import directory
    int lclSyncExit = 0;                    // loop check if db was created properly
    QString curDir = NULL;

    if(usrDir != QString(getenv("HOME"))){
        curDir = usrDir.absolutePath();  // get chosen path
        initLocalObj();

        //sync to db
        if(syncType == 0){  /// sync Audio
            // add main import folder
            addFolder(AudDirCount, curDir.toStdString(), usrDir.dirName().toStdString(), syncType);
            // scan main for directories
            scanDir(curDir, 0);
            scanFiles(0);
            dbCon.createLocalDB();  // write DB
            dbCon.writeDB(lclDirName, lclDir, lclDirID, lclDirPar, AudDirCount, 0);  // write song directories
            dbCon.writeDB(lclFileNames, lclFiles, lclFileID, lclFilePar, AudItemCount, 1);  // write song files
        }
        else{
            // add main import folder
            addFolder(VidDirCount, curDir.toStdString(), usrDir.dirName().toStdString(), syncType);
            scanDir(curDir, 1);
            scanFiles(1);
            dbCon.createLocalDB();  // write DB
            dbCon.writeDB(lclDirName, lclDir, lclDirID, lclDirPar, VidDirCount, 2);  // write song directories
            dbCon.writeDB(lclFileNames, lclFiles, lclFileID, lclFilePar, VidItemCount, 3);  // write song files
            lclSyncExit = 1;
        }
    }
}

/*
* Scan User Directory for folders. Add all discovered folders
*/
void localsync::scanDir(QString dir, int scanType){
    QDirIterator directories(dir, QDir::Dirs | QDir::NoDotAndDotDot);
    int count = 0;
    if(scanType == 0){
        count = AudDirCount;
    }
    else{
        count = VidDirCount;
    }
    while(directories.hasNext()){
        directories.next();
        addFolder(count, directories.filePath().toStdString(), directories.fileName().toStdString(), scanType);
        count++;
    }
}

/*
* Scan User Directory for folders. Add all discovered files
*/
void localsync::scanFiles(int scanType){
    int count = 0;
    int countSize = 0;
    if(scanType == 0){
        //   count = AudItemCount;
        countSize = AudDirCount;
    }
    else{
        //    count = VidItemCount;
        countSize = VidDirCount;
    }
    for(int i=0; i<countSize; i++){

        QDirIterator dirWalk(QString::fromStdString(lclDir[i]), QDir::Files | QDir::NoSymLinks);

        while(dirWalk.hasNext())
        {
            dirWalk.next();
            if(scanType == 0){  // if scanning audio
                if(dirWalk.fileInfo().completeSuffix() == "mp3" || dirWalk.fileInfo().completeSuffix() == "flac"){
                    addFile(count, dirWalk.filePath().toStdString(), dirWalk.fileName().toStdString(), lclDirID[i], scanType);
                    count++;
                }
            }
            else{   // if scanning video
                if(dirWalk.fileInfo().suffix() == "avi" || dirWalk.fileInfo().suffix() == "mp4"){
                    addFile(count, dirWalk.filePath().toStdString(), dirWalk.fileName().toStdString(), lclDirID[i], scanType);
                    count++;
                }
            }
        }
    }
}

void localsync::addFile(int count, string direc, string name, int par, int type){
    lclFileNames[count] = name;
    lclFiles[count] = direc;
    lclFilePar[count] = par;
    if(type == 0){
        lclFileID[count] = AudItemCount+AudioCount;
        AudItemCount++;
        if(AudItemCount >= AudItemINIT){
            initFiles(AudItemCount, 100, AudItemCount);
            AudItemINIT = AudItemCount + 100;
        }
    }
    else{
        lclFileID[count] = VidItemCount+VideoCount;
        VidItemCount++;
        if(VidItemCount >= VidItemINIT){
            initFiles(VidItemCount, 100, VidItemCount);
            VidItemINIT = VidItemCount + 100;
        }
    }
}

/*
 * Import Audio + Video Folder.  Types: 0) Audio. 1) Video.
 */
void localsync::addFolder(int count, string dir, string name, int type){
    lclDir[count] = dir;       /// full path
    lclDirName[count] = name;  /// folder name
    dbCon.getLastIDs(&AudFolderCount, &VidFolderCount, &AudioCount, &VideoCount);
    if(type == 0){
        if(AudDirCount == 0){          /// initial import folder
            lclDirPar[count] = 0;      /// parent to root
            lclDirID[count] = AudDirCount+AudFolderCount+5;
            parentID = AudDirCount+AudFolderCount+1;
            AudDirCount++;
        }
        else{
            lclDirPar[count] = parentID;
            lclDirID[count] = AudDirCount+AudFolderCount+1;

            AudDirCount++;
            if(AudDirCount >= AudDirINIT){
                initDirs(AudDirCount, 100, AudDirCount);
                AudDirINIT = AudDirCount + 100;
            }
        }
    }
    else{
        if(VidDirCount == 0){
            lclDirPar[count] = 0;      /// parent to root
            lclDirID[count] = VidDirCount+VidFolderCount+5;

            parentID = VidDirCount+VidFolderCount+1;
            VidDirCount++;
        }
        else{
            lclDirPar[count] = parentID;
            lclDirID[count] = VidDirCount+VidFolderCount+1;

            VidDirCount++;
            if(VidDirCount >= VidDirINIT){
                initDirs(VidDirCount, 100, VidDirCount);
                VidDirINIT = VidDirCount + 100;
            }

        }
    }
}
/*
* allocate new initial Directory array
*/
void localsync::initDirs(int beg, int end, int count){
    string *dirCopy, *dirNameCopy;
    int *dirParCopy, *dirIDCopy;
    if(beg !=0){    // if not the inital allocation
        ///allocate copy
        dirCopy = new string[count];
        dirNameCopy = new string[count];
        dirParCopy = new int[count];
        dirIDCopy = new int[count];
        for(int i=0; i<count; i++){
            dirCopy[i] = "-"; dirNameCopy[i] = "-";
            dirParCopy[i] = 0; dirIDCopy[i] = 0;
        }
        /// copy from resideFiles to resideCopy
        for(int i=0; i<count; i++){
            dirCopy[i] = lclDir[i]; dirNameCopy[i] = lclDirName[i];
            dirParCopy[i] = lclDirPar[i];dirIDCopy[i] = lclDirID[i];
        }
        delete [] lclDir; delete [] lclDirName;
        delete [] lclDirPar; delete [] lclDirID;
    }
    /// initialize new resideFileArray
    lclDir = new string[count+end];
    lclDirName = new string[count+end];
    lclDirPar = new int[count+end];
    lclDirID = new int[count+end];
    for(int i=0; i<end; i++){
        lclDir[i] = "-"; lclDirName[i] = "-";
        lclDirPar[i] = 0; lclDirID[i] = 0;
    }

    if(beg != 0){    // if not the inital allocation
        /// fill with contents of copy
        for(int i=0; i<count; i++){
            lclDir[i] = dirCopy[i];
            lclDirName[i] = dirNameCopy[i];
            lclDirPar[i] = dirParCopy[i];
            lclDirID[i] = dirIDCopy[i];
        }
    }
}

/*
* allocate new initial file array
*/
void localsync::initFiles(int beg, int end, int count){
    string *fileCopy, *fileNameCopy;
    int *fileIDCopy, *fileParCopy;
    if(beg !=0){    // if not the inital allocation
        ///allocate copy
        fileCopy = new string[count];
        fileNameCopy = new string[count];
        fileIDCopy = new int[count];
        fileParCopy = new int[count];
        for(int i=0; i<count; i++){
            fileCopy[i] = "-"; fileNameCopy[i] = "-";
            fileIDCopy[i] = 0; fileParCopy[i] = 0;
        }
        /// copy from resideFiles to resideCopy
        for(int i=0; i<count; i++){
            fileCopy[i] = lclFiles[i]; fileNameCopy[i] = lclFileNames[i];
            fileParCopy[i] = lclFilePar[i]; fileIDCopy[i] = lclFileID[i];
        }
        delete [] lclFiles; delete [] lclFileNames;
        delete [] lclFilePar; delete [] lclFileID;
    }
    /// initialize new resideFileArray
    lclFiles = new string[count+end];
    lclFileNames = new string[count+end];
    lclFilePar = new int[count+end];
    lclFileID = new int[count+end];
    for(int i=0; i<end; i++){
        lclFiles[i] = "-"; lclFileNames[i] = "-";
        lclFilePar[i] = 0;  lclFileID[i] = 0;
    }

    if(beg != 0){    // if not the inital allocation
        /// fill with contents of copy
        for(int i=0; i<count; i++){
            lclFiles[i] = fileCopy[i];
            lclFileNames[i] = fileNameCopy[i];
            lclFilePar[i] = fileParCopy[i];
            lclFileID[i] = fileIDCopy[i];
        }
    }
}




localsync::~localsync()
{
    delete [] lclFiles;
    delete [] lclFileNames;
    delete [] lclFilePar;
    delete [] lclFileID;
    delete [] lclDir;
    delete [] lclDirName;
    delete [] lclDirPar;
    delete [] lclDirID;
}
