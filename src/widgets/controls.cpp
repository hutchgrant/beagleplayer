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
    current.initFile(100);
    CON_MODE = 0;
    widget.setSeekSlider(ui->trackSlider);
    vol = new volume(this);
    ui->volLayout->addWidget(vol, 0,0,0,0,0);
    connect(vol, SIGNAL(volChanged(int)), this, SLOT(setVol(int)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(setTime()));
}

/*
  *  Control for Start of local File
  */
void controls::startLocal(char *finSong, char *finPath)
{
    secondCount = 0;
    minCount = 0;
    hourCount = 0;

    totalSecCount = 0;
    totalMinCount = 0;
    totalHourCount = 0;

    char *final;
    final = new char[strlen(finPath) + 10];
    sprintf(final, "%s", finPath);
    cout << "Final File Playing: " << final << endl;
    ui->songTitle->setText(QString(finSong));

    widget.show();
    widget.start(QStringList(final));
    connect(&widget, SIGNAL(stateChanged(int)), this, SLOT(stopTime(int)));
    connect(ui->trackSlider, SIGNAL(rangeChanged(int, int)), this, SLOT(rangeChange(int, int)));
}

/*
  *  Control for Start of local File
  */
void controls::start(string finSong, string finPath)
{
    char *final;
    final = new char[strlen(finPath.c_str()) + strlen(finSong.c_str()) + 10 ];
    sprintf(final, "%s%s", finPath.c_str(),finSong.c_str());
    cout << "Final File Playing: " << final << endl;
    widget.show();
    ui->songTitle->setText(QString(finSong.c_str()));
    widget.start(QStringList(final));
}

void controls::stopTime(int state){
    if(state == 5 || state == 0 || state == -1){ /// file paused/stopped/idle.
        timer.stop();
    }
    if(state == 3){  /// file playing
        timer.start(1000);
    }
}

void controls::rangeChange(int min, int max){
    totalMinCount = max / 60;
    totalHourCount = totalMinCount / 60;

    totalMinCount = totalMinCount - (totalHourCount *60);
    totalSecCount = max - (totalMinCount * 60);
}

void controls::setTimer(){
    stringstream playtime;
    string sHour = "", sMin = "", sSec = "";
    string sTotalHour = "", sTotalMin = "", sTotalSec = "";
    /*
     *  Set the default and current timer for file's time position
     */
    if(hourCount < 10){
        sHour = "0" +  QString("%1").arg(hourCount).toStdString();
    }else{
        sHour = QString("%1").arg(hourCount).toStdString();
    }
    if(minCount < 10){
        sMin = "0" +  QString("%1").arg(minCount).toStdString();
    }else{
        sMin = QString("%1").arg(minCount).toStdString();
    }
    if(secondCount < 10){
        sSec = "0" +  QString("%1").arg(secondCount).toStdString();
    }else{
        sSec = QString("%1").arg(secondCount).toStdString();
    }
    /*
     *  Set the default and current timer for file's end position
     */
    if(totalHourCount < 10){
        sTotalHour = "0" +  QString("%1").arg(totalHourCount).toStdString();
    }else{
        sTotalHour = QString("%1").arg(totalHourCount).toStdString();
    }
    if(totalMinCount < 10){
        sTotalMin = "0" +  QString("%1").arg(totalMinCount).toStdString();
    }else{
        sTotalMin = QString("%1").arg(totalMinCount).toStdString();
    }
    if(totalSecCount < 10){
        sTotalSec = "0" +  QString("%1").arg(totalSecCount).toStdString();
    }else{
        sTotalSec = QString("%1").arg(totalSecCount).toStdString();
    }
    if(widget.PlayingState){
        playtime <<  sHour << ":" << sMin << ":" << sSec << " / " <<  sTotalHour << ":" << sTotalMin << ":" << sTotalSec  ;
        ui->cntrl_time->setText(playtime.str().c_str());
    }
}
/*
  *  Sort the current list for
  */
void controls::startSelected(){
    int finSongSize = 0;
    int finPathSize = 0;
    int selID = 0;
    char *finPath;
    char *finSong;

    selID = curList[CurrentSelect];

    finSongSize = strlen(checkSongObjByID(selID, &current));
    finSong = new char[finSongSize + 1];
    finSong = checkSongObjByID(selID, &current);

    finPathSize = strlen(checkSongObjPathByID(selID, &current));
    finPath = new char[finPathSize + 1];
    finPath = checkSongObjPathByID(selID, &current);
    startLocal(finSong, finPath);
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
    current = fileObj();
    current.initFile(100);
    current = newList;
    curList = newIDlist;
}

/*
  * public slot for volume
  */
void controls::setVol(int vol){
    widget.setVolume(vol);

}

void controls::on_PAUSE_clicked()
{
    widget.pause();
}

void controls::on_PLAY_clicked()
{
    widget.play();
}

void controls::on_STOP_clicked()
{
    widget.stop();
    timer.stop();
}

void controls::on_NEXT_clicked()
{
    CurrentSelect++;
    startSelected();
}

void controls::on_PREV_clicked()
{
    CurrentSelect--;
    startSelected();
}

/*
 * Destructor
 */
controls::~controls()
{
    widget.close();
    delete ui;
}
