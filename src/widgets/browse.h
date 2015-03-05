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

#ifndef BROWSE_H
#define BROWSE_H

#include <QWidget>
#include <QStringListModel>
#include "src/object/fileobj.h"
#include "src/cache/cache.h"
#include "src/cache/localsync.h"
#include "src/widgets/radiostat.h"
#include "src/widgets/qweburl.h"

namespace Ui {
class browse;
}

class browse : public QWidget
{
    Q_OBJECT
    
public:
    int songCount, vidCount, radCount;
    localsync lclSync;
    cache *dbCon;

    explicit browse(QWidget *parent = 0);
    ~browse();
    void Sync(int mode);

    void initCache(cache *ini_cache){
        dbCon = ini_cache;
    }

signals:

    void curListChanged(fileObj &filelist, int *itemList, int amt);
    void trackChanged(string plName, string plPath, int plID, int plPar);
    void selectionChanged(int);                         //  a track was selected
    void FullSelection(int);                            // a track was double clicked
    void MenuSelection(int);                            // a Menu Item was selected
    void startTempTrack(string, string);                // for use with immediate, non-cached, open files

public slots:
    void updateTitle(int);                               // update Right ViewList
    void updateMenu();                                   // update Left ViewList
    void updateMode(int mode){                          // update view mode
        MenuMode = mode;
        updateMenu();
    }
private slots:

    void on_MenuList_clicked(const QModelIndex &index);
    void on_TrackList_clicked(const QModelIndex &index);
    void on_TrackList_doubleClicked(const QModelIndex &index);

private:
    int MenuMode;   /// 0  = Artist , 1 = VidDir
    fileObj Artist, Album, Song, VidDir, Video, RadioCat, Radio;
    fileObj RemArtist, RemAlbum, RemSong, RemVidDir, RemVideo;
    int *curSongID, *curVidID, *curRadID;
    Ui::browse *ui;
    QStringListModel *m_Model;
    QStringListModel *t_Model;

    QWebUrl *web;
    radiostat *radStat;
};

#endif // BROWSE_H
