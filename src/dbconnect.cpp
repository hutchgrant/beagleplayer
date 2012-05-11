#include "dbconnect.h"

dbconnect::dbconnect()
{
    pref.initDB();
}

QSqlDatabase dbconnect::OpenDB(){
    QSqlDatabase db2 = QSqlDatabase::addDatabase("QSQLITE");
    db2.setDatabaseName(pref.getSQL().c_str());
    if(!db2.open()){
        cout << "unable to connect with " << pref.getSQL().c_str() << endl;
    }
    return db2;
}
void dbconnect::closeDB(){

    QSqlDatabase::removeDatabase(pref.getSQL().c_str());
}

/*
* Create Local Database Table
*/
void dbconnect::createLocalDB() {
    string finalQry[7];
    finalQry[0] = "create table lcl_songdirs (key INTEGER PRIMARY KEY,lcl_dir_par integer,lcl_dir_name TEXT,lcl_dir_path TEXT)";
    finalQry[1] = "create table lcl_songs (key INTEGER PRIMARY KEY,lcl_dir_par integer,lcl_dir_name TEXT,lcl_dir_path TEXT)";
    finalQry[2] = "create table lcl_viddirs (key INTEGER PRIMARY KEY,lcl_dir_par integer,lcl_dir_name TEXT,lcl_dir_path TEXT)";
    finalQry[3] = "create table lcl_videos (key INTEGER PRIMARY KEY,lcl_dir_par integer,lcl_dir_name TEXT,lcl_dir_path TEXT)";
    finalQry[4] = "create table playlists (key INTEGER PRIMARY KEY,lcl_dir_par integer,lcl_dir_name TEXT,lcl_dir_path TEXT)";
    finalQry[5] = "create table playlist_items (key INTEGER PRIMARY KEY,lcl_dir_par integer,lcl_dir_name TEXT,lcl_dir_path TEXT)";
    for(int i=0; i<6; i++){
        writeMe(finalQry[i]);
    }
}

/*
* Write Local Database Folders to local database table  type: 0 Audio 1 Video
*/
void dbconnect::writeDB(fileObj &src, int itemCount, int type){
    int  pos = 0, posMax = 0, counter = 0, countRemind = 0;
    string dbTable;

    if(type == 0){  // directory
        dbTable = "lcl_songDirs";
    }
    else if(type ==1){
        dbTable = "lcl_songs";
    }
    else if(type == 2){
        dbTable = "lcl_vidDirs";
    }
    else if(type == 3){
        dbTable = "lcl_videos";
    }
    else if(type == 4){
        dbTable = "playlist_items";
    }
    string str2;
    counter = getMaxPos(itemCount);
    posMax = counter;
    /// reminder to get previous import folder position on second import
    for (int m = 0; m <= (itemCount / counter); m++) {
        countRemind = 0;
        stringstream os;
        for (int i = pos; i <= posMax; i++) {
            //   string strFin = lclD
            //            str2.erase(remove(str2.begin(), str2.end(), '\"'), str2.end());
            if (i != posMax && countRemind == 0) {
                os << " INSERT INTO "<< dbTable << " (lcl_dir_par,lcl_dir_name,lcl_dir_path) "
                   <<  "SELECT \"" << src.getPar(i) << "\" AS \"" << "lcl_dir_par" << "\", \""
                   <<  src.getName(i) << "\" AS \"" << "lcl_dir_name" << "\", \""
                   <<  src.getPath(i) << "\" AS \"" << "lcl_dir_path" << "\"";
                countRemind++;
            }
            if (i != posMax && countRemind != 0) {
                os << " UNION SELECT \""<< src.getPar(i) <<"\",\""<<src.getName(i) << "\",\""<< src.getPath(i) << "\"";

            } else if (i == posMax && countRemind != 0) {
                os << ";";
            }
        }
        str2 = os.str();
        writeMe(str2);
        posMax += counter;
        pos += counter;

        if (m == (itemCount / counter) - 1) {
            posMax = pos + itemCount - ((itemCount / counter)
                                       * counter);
        }
    }
}


/*
* Write Any string to database
*/
void dbconnect::writeMe(string qry){
    QSqlDatabase db = OpenDB();
    if(db.open()){
        QSqlQuery myQry;
        cout << qry << endl;
        myQry.prepare(qry.c_str());
        myQry.exec();

        db.close();
    }
}

/*
  * read database and fill object src
  */
void dbconnect::readDB(fileObj &src, string qry){
    src.initFile(100);
    QSqlDatabase db = OpenDB();
    QSqlQuery query;
    int count = 0;

    if(db.open()){
        query = QString(qry.c_str());

        while(query.next()){
            QString QValID = query.value(0).toString();   // ID
            QString QValPAR = query.value(1).toString();   // PARID
            QString QValNAME = query.value(2).toString();   // NAME
            QString QValPATH = query.value(3).toString();   // PATH

            if(QValNAME.toStdString() != "-"){
                src.set(count, QValID.toInt(), QValPAR.toInt(), QValNAME.toStdString().c_str(), QValPATH.toStdString().c_str());
                count++;
            }
    }
    closeDB();
    }
}

/*
  * Read database and fill objects for playlist
  */
void dbconnect::readPL(fileObj &playlist, fileObj &playlistItems ){

    readDB(playlist, "SELECT * FROM playlists");
    readDB(playlistItems, "SELECT * FROM playlist_items");
}

void dbconnect::getLastIDs(int *lastID){
    QSqlDatabase db = OpenDB();
    if(db.open()){
        QSqlQuery query;
        query = QString("SELECT * FROM playlists");

        while(query.next()){
            *lastID = query.value(0).toInt();
        }

    }
}

/*
  * Get last ID from last entry in database
  */
void dbconnect::getLastIDs(int *AudFolderCount, int *VidFolderCount, int *AudioCount, int *VideoCount){
    int IDcounter =0;
        QSqlDatabase db = OpenDB();
        if(db.open()){
            QSqlQuery query;
            for(int i=0; i<4; i++){
                IDcounter = i;
                if(IDcounter == 0){
                    query = QString("SELECT * FROM lcl_songdirs");
                }
                else if(IDcounter == 1){
                    query = QString("SELECT * FROM lcl_viddirs");
                }
                else if(IDcounter == 2){
                    query = QString("SELECT * FROM lcl_songs");
                }
                else if(IDcounter == 3){
                    query = QString("SELECT * FROM lcl_videos");
                }

                while (query.next()){
                    if(IDcounter == 0){
                        *AudFolderCount = query.value(0).toInt();
                    }
                    else if(IDcounter == 1){
                        *VidFolderCount = query.value(0).toInt();
                    }
                    else if(IDcounter == 2){
                        *AudioCount = query.value(0).toInt();
                    }
                    else if(IDcounter == 3){
                        *VideoCount = query.value(0).toInt();
                    }
                }
            }

            db.close();
        }
}


/*
  *  Get maximum write count
  */
int dbconnect::getMaxPos(int count) {
    int posMax;

    if (count < 400 && count > 100) {
        posMax = 100;
    } else if (count < 100 && count > 20) {
        posMax = 10;
    } else if (count < 20) {
        posMax = 5;
    } else if (count < 20 && count > 10) {
        posMax = 1;
    } else {
        posMax = 200;
    }
    return posMax;
}
