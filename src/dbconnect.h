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
    void readDB(fileObj &src, string qry);
    void writeMe(string qry);
    QSqlDatabase OpenDB();
    void closeDB();


    void createLocalDB();
    void writeDB(string *itemName, string *itemPath, int *itemID, int *itemPar, int itemCount, int type);
    void getLastIDs(int *AudFolderCount, int *VidFolderCount, int *AudioCount, int *VideoCount);
    void removeDir(int selected, int mode);
    int getMaxPos(int count);
    void getLastIDs();

};

#endif // DBCONNECT_H
