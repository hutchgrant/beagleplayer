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


#include "readdb.h"

readDB::readDB() {
}

QSqlDatabase readDB::OpenDB(){
    QSqlDatabase db2 = QSqlDatabase::addDatabase("QSQLITE");
    db2.setDatabaseName(DBLOCATE.c_str());
    if(!db2.open()){
        cout << "unable to connect with " << DBLOCATE.c_str() << endl;
    }
    return db2;
}

void readDB::closeDB(){

    QSqlDatabase::removeDatabase(DBLOCATE.c_str());

}

void readDB::LocalFill(fileObj &src, int type){
    src.initFile(100);
    QSqlDatabase db = OpenDB();
    int count = 0;
    if(db.open()){
        QSqlQuery query;

        /// sync Local
        if(type == 1){
            query = QString("SELECT * FROM lcl_songdirs");
        }
        else if(type == 2){
            query = QString("SELECT * FROM lcl_songs");
        }
        else if(type == 3){
            query = QString("SELECT * FROM lcl_viddirs");
        }
        else if(type == 4){
            query = QString("SELECT * FROM lcl_videos");
        }

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
    }
    closeDB();
}

readDB::~readDB(){
    closeDB();
}
