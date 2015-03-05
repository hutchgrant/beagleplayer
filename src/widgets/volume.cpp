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

#include "volume.h"
#include "ui_volume.h"
#include <QPainter>

/*
 * Constructor
 */
volume::volume(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::volume)
{
    ui->setupUi(this);
    curVol = 50;
}

/*
 * Destructor
 */
volume::~volume()
{
    delete ui;
}

/*
 *  When the volume slider is selected, emit the volume change to the control widget
 */
void volume::on_volSlider_sliderMoved(int position)
{
    if(position <= 0 ){
        curVol = 0;
    }
    else if(position >= 99){
        curVol = 100;
    }
    else if(curVol == position){
        return;
    }
    else{
        curVol = position;
    }
    update();
    emit volChanged(curVol);
}

/*
 *  Set the volume slider based on a slot received integer
 */
void volume::setPosition(int pos){
    ui->volSlider->setSliderPosition(pos);
}

/*
 * Basic example of a QPainter volume display, based on slider
 */
void volume::paintEvent(QPaintEvent * /* Event */ ){

    if(curVol > 60){
        QPainter paint2(this);
        paint2.setPen(Qt::NoPen);
        paint2.setBrush(Qt::red);
        paint2.translate(0,rect().height()-20);
        paint2.drawRect(QRect(40, 0, 50, 10));
    }
    if(curVol > 40){
        QPainter paint3(this);
        paint3.setPen(Qt::NoPen);
        paint3.setBrush(QColor(70, 150, 179, 200));
        paint3.translate(0, rect().height()-20);
        paint3.drawRect(QRect(30, 0, 40, 10));
    }
    if(curVol > 20){
        QPainter paint2(this);
        paint2.setPen(Qt::NoPen);
        paint2.setBrush(Qt::blue);
        paint2.translate(0,rect().height()-20);
        paint2.drawRect(QRect(20, 0, 30, 10));
    }
    if(curVol > 10){
        QPainter paint3(this);
        paint3.setPen(Qt::NoPen);
        paint3.setBrush(QColor(70, 150, 179, 200));
        paint3.translate(0, rect().height()-20);
        paint3.drawRect(QRect(10, 0, 20, 10));
    }
}
