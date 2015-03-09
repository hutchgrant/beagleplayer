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
#include "cache.h"

/*
 * Constructor
 */
cache::cache()
{
    cache_dir = "/.cache/beagleplayer2/";
    cache_dir = getenv("HOME") + cache_dir;
    db_file = cache_dir + "test.db";
    locate_file = cache_dir + "cache_locate.txt";
}
/*
 * Destructor
 */
cache::~cache(){

}

/*
 * Open SQLite DB
 */
void cache::openDB(){
   this->db = QSqlDatabase::addDatabase("QSQLITE", "beagle_cache");
   this->db.setDatabaseName(db_file.c_str());
}

/*
 * Close SQLite DB
 */

void cache::closeDB(){
    this->db = QSqlDatabase();
    QSqlDatabase::removeDatabase("beagle_cache");
}

/*
 * Initialize DB cache from text file, ensure tables are written
 */
void cache::init(){
    if(!initDB()){
        createCache();
        setInitDB();
        if(initDB()){
            addTables();
        }
    }
}

/*
* Write Any string to database
*/
int cache::writeMe(string qry){
       QSqlQuery myQry(db);
       myQry.prepare(qry.c_str());
       myQry.exec();
       stringstream os;

       os << "SELECT last_insert_rowid()";
       myQry.prepare(os.str().c_str());
       myQry.exec();
       return myQry.lastInsertId().toInt();
}

/*
 *  Write table with object
 */
int cache::writeDB(fileObj *file, string type){
    int lastid = 0;
    openDB();
    string fileName = "", filePath = "";

    if(this->db.open()){
        for(int x=0; x< file->getSize(); x++){
            stringstream os;
            fileName = sanitizeName(file->getName(x));
            filePath = sanitizeName(file->getPath(x));
            os << "INSERT INTO " << type << " (dir_par, dir_name, dir_path) VALUES ('" << file->getPar(x) << "', '" << fileName << "', '" <<  filePath << "')";
            lastid = writeMe(os.str());
        }
        this->db.close();
        closeDB();
    }
    return lastid;
}

/*
 *  Read Last Insert ID
 */
int cache::lastInsertID(){
    openDB();
    int lastid =0;
    if(this->db.open()){
         stringstream os;
         os << "SELECT last_insert_rowid()";
         QSqlQuery myQry(db);
         myQry.prepare(os.str().c_str());
         lastid = myQry.exec();
        this->db.close();
        closeDB();
    }
    return lastid;
}

/*
 *  Read from table to object
 */
void cache::readDB(fileObj &file, string type){
    stringstream os;
    int fileCount = 0;
    string fileName = "", filePath = "";
    openDB();
    if(this->db.open()){
        os << "SELECT * FROM "<< type << endl;
        QSqlQuery myQry(db);
        myQry.prepare(os.str().c_str());
        myQry.exec();
        while (myQry.next()){
            fileName = unsanitizeName(myQry.value(2).toString());
            filePath = unsanitizeName(myQry.value(3).toString());

            file.set(fileCount, myQry.value(0).toInt(), myQry.value(1).toInt(), fileName.c_str(), filePath.c_str());
            fileCount++;
        }
        this->db.close();
        closeDB();
    }
}

/*
 *  Read all objects at once
 */
void cache::readAll(fileObj &songdir, fileObj &songs, fileObj &viddir, fileObj &videos){
    readDB(songdir, "songdirs");
    readDB(songs, "songs");
    readDB(viddir, "viddirs");
    readDB(videos, "videos");
}

/*
 *  Delete from any table
 */
void cache::removeFrom(int key, string table, bool parent){
    stringstream os;
    openDB();
    if(this->db.open()){
        if(!parent){
            os << "DELETE FROM "<< table << " WHERE key=" << key << endl;
        }else{
            os << "DELETE FROM "<< table << " WHERE dir_par=" << key << endl;
        }
        QSqlQuery myQry(db);
        myQry.prepare(os.str().c_str());
        myQry.exec();
        this->db.close();
        closeDB();
    }
}

/*
 *  Add Initial Tables
 */
void cache::addTables(){
    openDB();
    this->db.open();
    string finalQry[8];
    finalQry[0] = "create table songdirs (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";
    finalQry[1] = "create table songs (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";
    finalQry[2] = "create table viddirs (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";
    finalQry[3] = "create table videos (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";
    finalQry[4] = "create table playlists (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";
    finalQry[5] = "create table playlist_items (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";
    finalQry[6] = "create table categories (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";
    finalQry[7] = "create table radios (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";
    finalQry[8] = "create table theme (key INTEGER PRIMARY KEY,dir_par integer,dir_name TEXT,dir_path TEXT)";

    for(int i=0; i<9; i++){
        writeMe(finalQry[i]);
     }
    this->db.close();
    closeDB();
}

/*
 *  Update from table to object
 */
int cache::updateDB(fileObj *file, string type){
    int lastid = 0;
    openDB();
    string fileName = "", filePath = "";

    if(this->db.open()){
        for(int x=0; x< file->getSize(); x++){
            stringstream os;
            fileName = sanitizeName(file->getName(x));
            filePath = sanitizeName(file->getPath(x));
            os << "UPDATE " << type << " SET dir_par='" << file->getPar(x) << "', dir_name='" << fileName << "', dir_path='" <<  filePath << "'";
            lastid = writeMe(os.str());
        }
        this->db.close();
        closeDB();
    }
    return lastid;
}

/*
 *  Locate cache and read text file in order to locate cache db, if it moved.
 */
bool cache::initDB(){
   bool found = false;
   char PrefIn[100];
   FILE* fp;
   string Cache = locate_file;
   fp = fopen(Cache.c_str(), "r");   /// open cached db location
   if(fp != NULL){
       cout << "cache found " << endl;
       rewind(fp);
       while(!feof(fp)){
           fscanf(fp, "%s", &PrefIn);
           found = true;
       }
   }
   else{
       return false;
   }

   if(found){   // if the db (listed in this text cache) exists
       db_file = PrefIn;  /// set SQL location
       fclose(fp);
       return true;
   }
   else{
       fclose(fp);
       return false;
   }
}

/*
 *   Initialize text file to hold caches location if it moves.  This file must stay in a static location
 */
void cache::setInitDB(){
   ofstream myfile;
   string cache = locate_file;
   qDebug() << "writing db : " << db_file.c_str() << " to file " << locate_file.c_str();
   myfile.open (cache.c_str());
   myfile << db_file.c_str();
   myfile.close();
}

/*
 * Create local cache directory
 */
void cache::createCache(){
   QString q_main = cache_dir.c_str();
   QDir(q_main).mkdir(q_main);
}

/*
 *  Remove any sanitize place holdings characters that would otherwise look ugly
 */
string cache::unsanitizeName(QString filename){
    filename.replace(QString("$_"), QString("'"));
    return filename.toStdString().c_str();
}

/*
 *  Sanitize any forbidden characters that would otherwise break our sql queries
 */
string cache::sanitizeName(QString filename){
    filename.replace(QString("'"), QString("$_"));
    return filename.toStdString().c_str();
}
