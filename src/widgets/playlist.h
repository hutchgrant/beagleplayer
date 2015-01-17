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
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QStringListModel>
#include "src/object/fileobj.h"
#include "src/cache/cache.h"
#include "newplaylist.h"
namespace Ui {
class playlist;
}

class playlist : public QWidget
{
    Q_OBJECT
    
public:

    cache *dbCon;
    fileObj pList, pListItems;
    fileObj pNewList, pNewItems;

    newplaylist plDialog;

    int PLMODE = 0, pListSelect = 0, pItemSelect = 0;
    int *curPLlist;
    string tempTrack = "", tempPath = "";


    explicit playlist(QWidget *parent = 0);
    virtual ~playlist();

    void createNewPL();
    void fillPL();
    bool readPL();

    void init();
    void initCache(cache *ini_cache){
        dbCon = ini_cache;
        readPL();
    }
    void addToNewPL(string track, string path);
    void addToCurrent(int parid, string track, string path);

public slots:
    /// if a track was selected elsewhere,
    /// Need that information immediately avail to playlist
    void setTempTrack(string track, string path){
        tempTrack = track;
        tempPath = path;
    }

signals:
    void playlistChanged(fileObj &plItem, int * plItemList);
    void playlistSelection(int selected);
    void playlistFullSelection(int selected);

private slots:

    void on_PLAYLIST_doubleClicked(const QModelIndex &index);
    void on_PLAYLIST_clicked(const QModelIndex &index);

    void on_open_tool_clicked();
    void on_add_tool_clicked();
    //void on_remove_tool_clicked();

private:
    Ui::playlist *ui;
    QStringListModel *pl_model;

};

#endif // PLAYLIST_H
