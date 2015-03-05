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

#include "skincntrl.h"
#include "ui_skincntrl.h"

/*
 * Constructor
 */
skincntrl::skincntrl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::skincntrl)
{
    ui->setupUi(this);

    vol = new volume(this);
    ui->volLayout->addWidget(vol, 0,0,0,0,0);

    connect(vol, SIGNAL(volChanged(int)), this, SLOT(setVol(int)));
    connect(this, SIGNAL(setVolume(int)), vol, SLOT(setPosition(int)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(setTime()));
    connect(ui->trackSlider, SIGNAL(rangeChanged(int, int)), this, SLOT(rangeChange(int, int)));
    connect(ui->trackSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
}

/*
 *  Stop/start the media play timer based on the control widget's state
 */
void skincntrl::stopTime(int state){
    if(state == 5 || state == 0 || state == -1){ /// file paused/stopped/idle.
        timer.stop();
    }
    if(state == 3){  /// file playing
        timer.start(1000);
    }
}

/*
 *  Set the media play timer's max time based on the sliderRange given by qmpwidget
 *  The max range is provided in seconds, we only need to divide it into hours, min, seconds
 */
void skincntrl::rangeChange(int min, int max){
    totalMinCount = max / 60;
    totalHourCount = totalMinCount / 60;

    totalMinCount = totalMinCount - (totalHourCount *60);
    totalSecCount = max - (totalMinCount * 60);
}


/*
 *  Set the media play timer's min time based on a position the slider is moved to.
 *  This integer is in seconds, we only need to divide it into hours, min, seconds
 */
void skincntrl::sliderMoved(int pos){
    minCount = pos / 60;
    hourCount = minCount / 60;

    minCount = minCount - (hourCount *60);
    secondCount = pos - (minCount * 60);
    emit remConSeek(pos);
}

/*
 *  Control the Shuffle, timer display and functionality
 */
void skincntrl::playlistControl(){
    string playtime;
    controls cont;
    if(hourCount >= totalHourCount && minCount >= totalMinCount && secondCount >= totalSecCount){
       timer.stop();
    }
    playtime = cont.getTimeDisplay(hourCount, minCount, secondCount, totalHourCount, totalMinCount, totalSecCount);
    ui->cntrl_time->setText(playtime.c_str());
}

/*
 *  Initialize timer
 */
void skincntrl::initTrack(){
    secondCount = 0;
    minCount = 0;
    hourCount = 0;

    totalSecCount = 0;
    totalMinCount = 0;
    totalHourCount = 0;
    ui->trackSlider->setSliderPosition(0);
}

/*
  * public slots
  */
/// Set Track to control widget's track name
void skincntrl::setTrack(string track){
    initTrack();
    ui->songTitle->setText(track.c_str());
    timer.start(1000);
}
/// Set slider to the seek position of the control widget
void skincntrl::setSeekPos(int pos){
    ui->trackSlider->setSliderPosition(pos);
    sliderMoved(pos);
}
/// Set the max range of media file timer, based on the control widget
void skincntrl::setSeekRange(int max){
    ui->trackSlider->setRange(0, max);
}
/// Set the volume of the volume widget, based on the control widget
void skincntrl::setMainVol(int vol){
    emit setVolume(vol);
}

/*
* public signals
*/
/// Set the Control widget, based on the volume
void skincntrl::setVol(int vol){
    emit remConVol(vol);
}
/// Set the Control widget to PAUSE
void skincntrl::on_PAUSE_clicked()
{
    emit remConFile(2);
}
/// Set the Control widget to PLAY
void skincntrl::on_PLAY_clicked()
{
    emit remConFile(1);
}
/// Set the Control widget to STOP, and stop the timer
void skincntrl::on_STOP_clicked()
{
    timer.stop();
    emit remConFile(3);
}
/// Set the Control widget to move to the NEXT track
void skincntrl::on_NEXT_clicked()
{
    emit remConFile(4);
}
/// Set the Control widget to move to the NEXT track
void skincntrl::on_PREV_clicked()
{
    emit remConFile(5);
}

/*
 * Destructor
 */
skincntrl::~skincntrl()
{
    delete ui;
}
