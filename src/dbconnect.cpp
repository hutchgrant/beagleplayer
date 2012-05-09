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
    string finalQry[4];
    finalQry[0] = "create table lcl_songdirs (key INTEGER PRIMARY KEY,lcl_dir_name TEXT,lcl_dir_path TEXT,lcl_dir_id integer,lcl_dir_par integer,lcl_dir_type TEXT)";
    finalQry[1] = "create table lcl_songs (key INTEGER PRIMARY KEY,lcl_dir_name TEXT,lcl_dir_path TEXT,lcl_dir_id integer,lcl_dir_par integer,lcl_dir_type TEXT)";
    finalQry[2] = "create table lcl_viddirs (key INTEGER PRIMARY KEY,lcl_dir_name TEXT,lcl_dir_path TEXT,lcl_dir_id integer,lcl_dir_par integer,lcl_dir_type TEXT)";
    finalQry[3] = "create table lcl_videos (key INTEGER PRIMARY KEY,lcl_dir_name TEXT,lcl_dir_path TEXT,lcl_dir_id integer,lcl_dir_par integer,lcl_dir_type TEXT)";
    for(int i=0; i<4; i++){
        writeMe(finalQry[i]);
    }
}

/*
* Write Local Database Folders to local database table  type: 0 Audio 1 Video
*/
void dbconnect::writeDB(string *itemName, string *itemPath, int *itemID, int *itemPar, int itemCount, int type){
    int  pos = 0, posMax = 0, counter = 0, countRemind = 0, dirCount;
    string dbTable, itemType;

    if(type == 0){  // directory
        dbTable = "lcl_songDirs";
        itemType = "folder";
    }
    else if(type ==1){
        dbTable = "lcl_songs";
        itemType = "song";
    }
    else if(type == 2){
        dbTable = "lcl_vidDirs";
        itemType = "folder";
    }
    else if(type == 3){
        dbTable = "lcl_videos";
        itemType = "video";
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
                os << " INSERT INTO "<< dbTable << " (lcl_dir_name,lcl_dir_path,lcl_dir_id,lcl_dir_par,lcl_dir_type) "
                   <<  "SELECT \"" << itemName[i] << "\" AS \"" << "lcl_dir_name" << "\", \""
                   <<  itemPath[i] << "\" AS \"" << "lcl_dir_path" << "\", \""
                   <<  itemID[i] << "\" AS \"" << "lcl_dir_id" << "\", \""
                   <<  itemPar[i] << "\" AS \"" << "lcl_dir_par" << "\", \""
                   <<  itemType << "\" AS \"" << "lcl_dir_type" << "\"";
                countRemind++;
            }
            if (i != posMax && countRemind != 0) {
                os << " UNION SELECT \""<< itemName[i] <<"\",\""<< itemPath[i] << "\",\"" << itemID[i] << "\",\""<< itemPar[i] << "\",\""<< itemType << "\"";

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
            QString QVal1 = query.value(1).toString();
            QString QVal2 = query.value(2).toString();
            QString QVal3 = query.value(3).toString();
            QString QVal4 = query.value(4).toString();

            if(QVal1.toStdString() != "-"){
                src.set(count, QVal3.toInt(), QVal4.toInt(), QVal1.toStdString().c_str(), QVal2.toStdString().c_str());
                count++;
            }
    }
    closeDB();
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
                        *AudFolderCount = query.value(3).toInt();
                    }
                    else if(IDcounter == 1){
                        *VidFolderCount = query.value(3).toInt();
                        cout << *VidFolderCount << endl;
                    }
                    else if(IDcounter == 2){
                        *AudioCount = query.value(3).toInt();
                    }
                    else if(IDcounter == 3){
                        *VideoCount = query.value(3).toInt();
                    }
                }
            }

            db.close();
        }
}


/*
  * Remove Directory from DB
  */ /*
void dbconnect::removeDir(int selected, int mode){
    char *myQry;
    if(selected >= 0){
        if(mode == 1){
            if(selected < AudDirCount){
                myQry = new char[lclDirName[selected].length()+100];
                sprintf(myQry, "DELETE from lcl_dirs WHERE lcl_dir_name='%s'", lclDirName[selected].c_str());
                string finQry = string(myQry);
                writeMe(finQry);
            }
        }
        else{
            if(selected < VidDirCount){
                myQry = new char[lclDirName[selected].length()+100];
                sprintf(myQry, "DELETE from lcl_dirs WHERE lcl_dir_name='%s'", lclDirName[selected].c_str());
                string finQry = string(myQry);
                writeMe(finQry);
            }
        }
    }
}
*/

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
