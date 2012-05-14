#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QtSql>
#include "preferences.h"
#include "fileobj.h"
class dbconnect
{
public:
    preferences pref;
    dbconnect();
    virtual ~dbconnect();
    void readDB(fileObj &src, string qry);
    void readPL(fileObj &playlists, fileObj &playlistItems);
    void writeMe(string qry);
    QSqlDatabase OpenDB();
    void closeDB();


    void createLocalDB();
    void writeDB(fileObj &src, int itemCount, int type);
    void getLastIDs(int *lastID);
    void getLastIDs(int *AudFolderCount, int *VidFolderCount);
    void removeDir(int selected, int mode);
    int getMaxPos(int count);
    void getLastIDs();
};

#endif // DBCONNECT_H
