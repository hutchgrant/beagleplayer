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

#ifndef skincntrl_H
#define skincntrl_H

#include <QWidget>
#include <QDebug>
#include <sstream>
#include "src/widgets/controls.h"
#include "src/object/fileobj.h"
#include "src/qmpwidget/qmpwidget.h"
#include "src/widgets/volume.h"

namespace Ui {
class skincntrl;
}

class skincntrl : public QWidget
{
    Q_OBJECT

public:

    int secondCount, minCount, hourCount;
    int totalSecCount, totalMinCount, totalHourCount;
    string track;

    QTimer timer;

    skincntrl(QWidget *parent = 0);
    virtual ~skincntrl();

    void startLocal(char *finSong, char *finPath);
    void start(string finSong, string finPath);
    void startSelected();
    void playlistControl();

    void initTrack();
    void setTimer();

public slots:
    void setVol(int vol);

private slots:

    void on_PAUSE_clicked();

    void on_PLAY_clicked();

    void on_STOP_clicked();

    void on_NEXT_clicked();

    void on_PREV_clicked();

    void setTime(){
        secondCount++;
        if(secondCount % 60 == 0){
            secondCount = 0;
            minCount++;
        }
        if(minCount % 60 == 0){
            minCount = 0;
            hourCount = minCount / 60;
        }
        playlistControl();
    }

    void setTrack(string);
    void setSeekPos(int);
    void setSeekRange(int);
    void setMainVol(int);

    void stopTime(int);
    void rangeChange(int, int);
    void sliderMoved(int);  // user seeked the f

signals:
    void setVolume(int);
    void remConSeek(int);
    void remConFile(int);
    void remConVol(int);
private:
        void adjustVol(int vol);
        int CurrentSelect;  /// current selection number
        int *curList;       /// current cue list ID's
        Ui::skincntrl *ui;
        string name, path;
        volume *vol;
};

#endif // skincntrl_H
