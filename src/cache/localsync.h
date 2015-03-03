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

#ifndef LOCALSYNC_H
#define LOCALSYNC_H
#include <QDir>
#include <QStringListModel>
#include <QDirIterator>
#include <QMainWindow>
#include <qstringlist.h>
#include <QMainWindow>
#include <QFileDialog>
#include <iostream>
#include "src/object/fileobj.h"
#include <sstream>
#include "cache.h"

using namespace std;

class localsync
{
    cache dbCon;
    fileObj localDir, localFile;
public:
    localsync();
    virtual ~localsync();
    void scanFiles(int scanType, int folderCount);
    void scanDir(QString dir, int scanType);
    void Sync(QDir usrDir, int syncType);
    string sanitizeName(QString filename);
    void addItem(int itemType, int pos, int id, int par, QString fileName, QString filePath);


private:
    QDir mydir;
};

#endif // LOCALSYNC_H
