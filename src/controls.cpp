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

#include "controls.h"
#include "ui_controls.h"

controls::controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controls)
{
    ui->setupUi(this);
    CON_MODE = 0;
}
/*
  *  Control for Start of local File
  */
void controls::startLocal(char *finSong, char *finPath)
{
    char *final;
    final = new char[strlen(finPath) + 10];
    sprintf(final, "%s", finPath);
    cout << "Final File Playing: " << final << endl;
    widget.show();
    widget.start(QStringList(final));
}

/*
  *  Sort the current list for
  */
void controls::startSelected(){
    int FinParentID = 0;
    int finSongSize = 0;
    int finPathSize = 0;
    int selID = 0;
    char *finPath;
    char *finSong;


    selID = curList[CurrentSelect];

    finSongSize = strlen(checkSongObjByID(selID, current));
    finSong = new char[finSongSize + 1];

    FinParentID = checkSongObjParByID(selID,current);
    finSong = checkSongObjByID(selID, current);

        finPathSize = strlen(checkSongObjPathByID(selID, current));
        finPath = new char[finPathSize + 1];
        finPath = checkSongObjPathByID(selID, current);
        cout << "final song = " << finSong << endl;
        cout << "final Path = " << finPath << endl;
        startLocal(finSong, finPath);

}
/*
  *When Stop Button is pressed
  */
void controls::on_STOP_but_clicked()
{
    widget.stop();
}


/*
  * When Pause button clicked, pause
  */
void controls::on_PAUSE_but_clicked()
{
    widget.pause();
}

/*
  * Slot to Change connection Mode
  */
void controls::changeCon(int mode){
    CON_MODE = mode;
}

/*
  * Set Current qeue list when list changes
  */
void controls::setCurList(fileObj &newList, int *newIDlist){
    current = newList;
    curList = newIDlist;
}

/*
  * public slot for volume
  */
void controls::setVol(int vol){
    widget.setVolume(vol);

}
controls::~controls()
{
    delete ui;
}

