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

#ifndef CONTROLS_H
#define CONTROLS_H

#include <QWidget>
#include <QDebug>
#include <sstream>
#include "src/object/fileobj.h"
#include "src/qmpwidget/qmpwidget.h"
#include "src/widgets/volume.h"
#include "src/widgets/detached.h"

namespace Ui {
class controls;
}

class controls : public QWidget
{
    Q_OBJECT

public:

    int secondCount, minCount, hourCount;
    int totalSecCount, totalMinCount, totalHourCount;
    int pl_selected;

    QTimer timer;
    QMPwidget widget;

    controls(QWidget *parent = 0);
    virtual ~controls();

    void initSlider();
    void initPlaylist();
    void startLocal(char *finSong, char *finPath);
    void start(string finSong, string finPath);
    void startSelected();

    string getTimeDisplay(int hourCount, int minCount, int secondCount, int totalHourCount, int totalMinCount, int totalSecCount);
    void playlistControl();
    void close(){
        widget.close();
    }

public slots:
    void setVol(int vol);
    void setSelection(int selection){
        CurrentSelect = selection;
    }
    void setTempTrackAndPlay(string track, string path){
        start(track, path);
    }
    void setSelectionAndPlay(int selection){
        CurrentSelect = selection;
        initPlaylist();
        startSelected();
    }
    void setCurList(fileObj &newlist, int * newIDlist,int amt, bool range);

    /*
     * Remote commands for signals from another widget!
     * connect = 1  - play
     * connect = 2 - pause
     * connect = 3 - stop
     * connect = 4 - next
     * connect = 5 - prev
     */
    void remoteCommand(int connect){
        if(connect == 3){
            widget.stop();
            timer.stop();
            qDebug() << "remote stop";
        }else if(connect == 2){
            qDebug() << "remote pause";
            timer.stop();
            widget.pause();
        }else if(connect == 1){
            qDebug() << "remote play";
            timer.start(1000);
            widget.pause();
        }else if(connect == 4){
            qDebug() << "remote next";
            CurrentSelect++;
            startSelected();
        }else if(connect == 5){
            qDebug() << "remote prev";
            CurrentSelect--;
            startSelected();
        }
    }

    void remoteVolume(int vol){
        setVol(vol);
    }
    void remoteSeek(int pos);

    QString getTrack(){
        return  QString(current.getName(CurrentSelect));
    }

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
        emit remConSeek(secondCount);
        playlistControl();
    }

    void stopTime(int);
    void rangeChange(int, int);
    void sliderMoved(int);  // user seeked the f
    void on_detach_clicked(){
       // emit detachControls();
        detach->setOrientation(Html5ApplicationViewer::ScreenOrientationLockLandscape);
        detach->setMaximumSize(QSize(640, 480));
        detach->showMaximized();
        detach->loadUrl(QUrl("qrc:///template/template/template.html"));
    }

signals:
    void detachControls();
    void songChanged(string);
    void setVolume(int);
    void remConSeek(int);
    void remConRange(int);
    void remConFile(int);
    void remConVol(int);
private:
        void adjustVol(int vol);
        int CurrentSelect;  /// current selection number
        int *curList, *announcedList;       /// current cue list ID's
        int curAmount, announcedAmount; /// init for playlist size
        bool curRange, announcedRange; /// init for ignoring range
        fileObj current, announced;    /// current fileObj List
        detached *detach;
        Ui::controls *ui;
        string name, path;
        volume *vol;
};

#endif // CONTROLS_H
