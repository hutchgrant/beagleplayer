#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QtSql>
#include "fileobj.h"
#include "prefdialog.h"
#include <iostream>
#include <sstream>
#include <fstream>
#define TEMPCACHE "/.cache/beagleplayer/dbcache.txt"

using namespace std;
class dbconnect
{
public:
    string DBlocation;
    PrefDialog prefDg;

    dbconnect();
    dbconnect(const dbconnect& src);
    dbconnect& operator=(const dbconnect& src);
    virtual ~dbconnect();

    void control();
    void createLocalDB();
    void createCache();
    void setInitDB();

    QSqlDatabase OpenDB();
    bool initDB();

    void readDBpref();
    void readLocal(fileObj &Artist, fileObj &Song, fileObj &VidDir, fileObj &Video);
    void readDB(fileObj &src, string qry);
    void readPL(fileObj &playlists, fileObj &playlistItems);

    void writeDB(fileObj &src, int itemCount, int type);
    void writeMe(string qry);
    void writeDBpref();

    void removeDir(int selected, int mode);
    void removePL(int selected, int mode);
    void closeDB();

    void getLastIDs(int *lastID);
    void getLastIDs(int *AudFolderCount, int *VidFolderCount);
    int getMaxPos(int count);
    void getLastIDs();

    void setSQL(string strLocate){
       DBlocation = strLocate;
    }
    string getSQL(){
        return DBlocation;
    }


};

#endif // DBCONNECT_H
