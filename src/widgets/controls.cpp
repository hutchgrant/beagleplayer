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

/*
 * Constructor
 */
controls::controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controls)
{
    ui->setupUi(this);
    PlayMode = 0;
    curAmount = 0;
    announcedAmount = 0;
    curRange = false;
    announcedRange = false;

    detach = new detached();
    detachOpen = false;
    screenSz = QSize(1024,740);
    screenMode = false;
    themePath = "";

    current.initFile(100);

    vol = new volume(this);
    ui->volLayout->addWidget(vol, 0,0,0,0,0);

    connect(vol, SIGNAL(volChanged(int)), this, SLOT(setVol(int)));
    connect(this, SIGNAL(setVolume(int)), vol, SLOT(setPosition(int)));

    connect(&timer, SIGNAL(timeout()), this, SLOT(setTime()));

    connect(ui->trackSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));

    connect(this, SIGNAL(songChanged(string, string, int)), detach, SLOT(setTrack(string, string, int)));
    connect(this, SIGNAL(remConState(int)), detach, SLOT(setState(int)) );

    connect(this, SIGNAL(remConSeek(int)), detach, SLOT(setSeekPos(int)) );
    connect(this, SIGNAL(remConVol(int)), detach, SLOT(setVolume(int)) );

    /// connect detach player to controls
    connect(detach, SIGNAL(remConRange(int)), this, SLOT(rangeChange(int)));
    connect(detach, SIGNAL(remConSeek(int)), this, SLOT(remoteSeek(int)));
    connect(detach, SIGNAL(remConFile(int)), this, SLOT(remoteCommand(int)));
    connect(detach, SIGNAL(remConVol(int)), this, SLOT(remoteVolume(int)));
    connect(detach, SIGNAL(remScreenToggle(bool)), this, SLOT(remoteScreenToggle(bool)));
    connect(detach, SIGNAL(detachClose()), this, SLOT(detachExited()));
}

/*
 * Init slider
 */
void controls::initSlider(){
    PlayingState =0;
    secondCount = 0;
    minCount = 0;
    hourCount = 0;
    totalSecCount = 0;
    totalMinCount = 0;
    totalHourCount = 0;
    overallPos = 0;
    ui->trackSlider->setSliderPosition(0);
}

/*
 * Init playlist
 */
void controls::initPlaylist(){
    curRange = announcedRange;
    curAmount = announcedAmount;
    curList = new int[announcedAmount];
    curList = announcedList;

    current = fileObj();
    current.initFile(100);
    current = announced;
}

/*
  *  Control for Start of local File
  */
void controls::start(string finSong, string finPath)
{
    initSlider();
    PlayingState = 1;
    cout << "Final File Playing: " << finSong << endl;
    ui->songTitle->setText(QString(finSong.c_str()));
    timer.start(1000);
    openPlayer();
    emit songChanged(finSong, finPath, PlayMode);
}

/*
 * Stop Timer if widget stops
 */
void controls::controlTime(){
    if(PlayingState == 3 || PlayingState == 2 || PlayingState == -1){ /// file paused/stopped/idle.
        timer.stop();
        emit remConState(3);
    }
    if(PlayingState == 1){  /// file playing
        timer.start(1000);
         emit remConState(1);
    }
}

/*
 * Set Timer Max Range based on the range of widget slider
 */
void controls::rangeChange(int max){
    totalMinCount = max / 60;
    totalHourCount = totalMinCount / 60;

    totalMinCount = totalMinCount - (totalHourCount *60);
    totalSecCount = max - (totalMinCount * 60);
    ui->trackSlider->setRange(0, max);
}

/*
 * When the slider moves, calculate the new time, set detached slider position too.
 */
void controls::sliderMoved(int pos){
    minCount = pos / 60;
    hourCount = minCount / 60;

    minCount = minCount - (hourCount *60);
    secondCount = pos - (minCount * 60);
    overallPos = pos;
}

/*
 *  Playlist Control
 */
void controls::playlistControl(){
    string playtime;
    playtime = this->getTimeDisplay(hourCount, minCount, secondCount, totalHourCount, totalMinCount, totalSecCount);
    if(PlayingState == 1){
        ui->cntrl_time->setText(playtime.c_str());
        if(hourCount >= totalHourCount && minCount >= totalMinCount && secondCount >= totalSecCount && !curRange){
            timer.stop();
            CurrentSelect++;
            if(CurrentSelect < curAmount){
                startSelected();
                PlayingState = 1;
            }else{
                PlayingState = 3;
            }
        }else{
            PlayingState = 1;
        }
    }else{
        timer.stop();
        PlayingState = 3;
    }
    emit remConState(PlayingState);
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
    start(finSong, finPath);
}

/*
 *  Open html5 media player
 */
void controls::openPlayer(){
    if(!detachOpen){
        QIcon icon;
        detachOpen = true;
        detach->setOrientation(Html5ApplicationViewer::ScreenOrientationLockLandscape);

        if(screenMode){
            screenSz = QSize(1800,1000);
            detach->setScreenMode(true);
            detach->showFullScreen();
        }else{
            screenSz = QSize(1024,840);
            detach->setScreenMode(false);
            detach->showNormal();
        }
        detach->setMinimumSize(screenSz);
        icon.addFile("qrc:///res/beagleplayer_icon.png");
        detach->setWindowIcon(icon);
        detach->loadFile(themePath.c_str());
    }
}

/*
  * Set Current qeue list when list changes
  */
void controls::setCurList(fileObj &newList, int *newIDlist, int amount, bool range, int mode){
    announced = fileObj();
    announced.initFile(100);
    announced = newList;
    announcedList = newIDlist;
    announcedAmount = amount;
    announcedRange = range;
    PlayMode = mode;
}

////  slot for seeking from detached track slider
void controls::remoteSeek(int pos){
    minCount = pos / 60;
    hourCount = minCount / 60;

    minCount = minCount - (hourCount *60);
    secondCount = pos - (minCount * 60);
    overallPos = pos;
    ui->trackSlider->setSliderPosition(overallPos);
}

////  slot for seeking too the detached volume slider
void controls::setVol(int vol){
    emit setVolume(vol);
    emit remConVol(vol);
}
//// pause media widget
void controls::on_PAUSE_clicked()
{
    PlayingState = 2;
    controlTime();
}
//// play media widget
void controls::on_PLAY_clicked()
{
    PlayingState = 1;
    controlTime();
}
//// stop media widget + timer
void controls::on_STOP_clicked()
{
    PlayingState = 3;
    controlTime();
}
//// select next media file, start playback
void controls::on_NEXT_clicked()
{
    if(CurrentSelect < curAmount-1){
         CurrentSelect++;
        startSelected();
        PlayingState = 1;
        emit remConState(PlayingState);
    }
}
//// select previous media file, start playback
void controls::on_PREV_clicked()
{
     if(CurrentSelect > 0){
             CurrentSelect--;
             startSelected();
             PlayingState = 1;
             emit remConState(PlayingState);
     }
}

/*
 *  Return the display timer, based on hour, min, second for min and max ranges
 * convert int ranges to stringstream
 */
string controls::getTimeDisplay(int hourCount, int minCount, int secondCount, int totalHourCount, int totalMinCount, int totalSecCount){
    string sHour = "", sMin = "", sSec = "";
    string sTotalHour = "", sTotalMin = "", sTotalSec = "";
    stringstream playtime;

    /// Set the default and current timer for file's time position
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

    /// Set the default and current timer for file's end position
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
    playtime <<  sHour << ":" << sMin << ":" << sSec << " / " <<  sTotalHour << ":" << sTotalMin << ":" << sTotalSec  ;
    return playtime.str().c_str();
}

/*
 * Destructor
 */
controls::~controls()
{
    delete ui;
}
