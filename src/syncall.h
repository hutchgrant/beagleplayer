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


#ifndef SYNCALL_H
#define SYNCALL_H
#include "localsync.h"
#include <stdlib.h>
#include <stdio.h>
#include <QWidget>
#include <QFileDialog>
#include "preferences.h"
#include "prefdialog.h"
#include "dbconnect.h"

namespace Ui
{
    class syncAll;
}
class syncAll : public QMainWindow
{
    Q_OBJECT

public:
    localsync lclSync;
    dbconnect dbCon;
    preferences pref;
    PrefDialog prefDg;
    syncAll();

    void fillObjects(fileObj &Artist, fileObj &Song, fileObj &VidDir, fileObj &Video);
    void control(fileObj &Artist, fileObj &Song, fileObj &VidDir, fileObj &Video, int rwmode);
    void setPref();
    void dispPref();
};

#endif // SYNCALL_H
