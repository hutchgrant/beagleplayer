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
#include "fileobj.h"
#include "qmpwidget.h"
#include "volume.h"
namespace Ui {
class controls;
}

class controls : public QWidget
{
    Q_OBJECT

public:
    explicit controls(QWidget *parent = 0);
    virtual ~controls();
    QMPwidget widget;
    int pl_selected;

    void startSong(char *FinSong, int selID);
    void startLocal(char *finSong, char *finPath);
    void close(){
        widget.close();
    }
    void startSelected();
public slots:
    void setVol(int vol);
    void setSelection(int selection){
        CurrentSelect = selection;
    }

    void setSelectionAndPlay(int selection){
        CurrentSelect = selection;
        startSelected();
    }

    void setCurList(fileObj &newlist, int *newIDlist);
    void changeCon(int mode);

private slots:

    void on_PAUSE_clicked();

    void on_PLAY_clicked();

    void on_STOP_clicked();

    void on_NEXT_clicked();

    void on_PREV_clicked();

private:
        void adjustVol(int vol);
        int CurrentSelect;  /// current selection number
        int CON_MODE;       /// connection mode
        int *curList;       /// current cue list ID's
        fileObj current;    /// current fileObj List
        Ui::controls *ui;
        string name, path;
        volume *vol;
};

#endif // CONTROLS_H
