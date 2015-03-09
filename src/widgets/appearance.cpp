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
#include "appearance.h"
#include "ui_appearance.h"

/*
 *  Constructor
 */
appearance::appearance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::appearance)
{
    ui->setupUi(this);
    themes = new fileObj();
}

/*
 *  Get all themes from ./themes folder
 */
void appearance::getThemes(){
    int themeCount = 0;
    int count = 0;
    fileObj themeDir = fileObj();
    QDirIterator directories(QDir::currentPath().append("/res/themes/"), QDir::Dirs | QDir::NoDotAndDotDot);
    while(directories.hasNext()){
        directories.next();
        themeDir.set(0, count, 0, directories.fileName().toStdString().c_str(), directories.filePath().toStdString().c_str());
        count++;
    }

       for(int x=0; x< themeDir.getSize(); x++){
        QDirIterator dirWalk(QString::fromStdString(themeDir.getPath(x)), QDir::Files | QDir::NoSymLinks);
        while(dirWalk.hasNext())
        {
           dirWalk.next();
           if(dirWalk.fileInfo().completeSuffix() == "theme"){
              qDebug() << dirWalk.fileName() << endl;
              qDebug() << dirWalk.filePath() << endl;
              themes->set(themeCount,0,0,dirWalk.fileName().toStdString().c_str(), dirWalk.filePath().toStdString().c_str());
              themeCount++;
           }
        }
       }
}

/*
 *  Set the selected theme's image to the appearance display
 */
void appearance::setThemeImg(string imgPath){
    QGraphicsScene *scn = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene( scn );

    QPixmap someImage(imgPath.c_str());
    scn->setSceneRect(0,0,0,0);
    scn->addPixmap(someImage);
    ui->graphicsView->show();
}

/*
 *  Parse .theme file for appearance display var
 */
void appearance::parseTheme(string filename){
    QFile file(filename.c_str());
    QStringList strings = QStringList();
    int count =0;
    bool exit = false;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd() || !exit) {
            if(count < 6){
                strings += in.readLine().split("=").at(1);
            }else{
                exit = true;
            }
            count++;
        }
        file.close();
    }
    ui->theme_selected->setText(strings.at(0));
    ui->theme_author->setText(strings.at(2));
    ui->theme_version->setText(strings.at(3));
    ui->theme_title->setText(strings.at(1));
    ui->theme_date->setText(strings.at(4));
    ui->theme_desc->setText(strings.at(5));
}

/*
 *  Fill Themes in list
 */
void appearance::fillThemes(){
  //  cah->readDB(themes, "themes");
    QStringList themeMenu;
    t_Model = new QStringListModel(this);
    for(int i=0; i <= themes->getSize(); i++){
            themeMenu << themes->getName(i);
     }
    t_Model->setStringList(themeMenu);
    ui->theme_list->setModel(t_Model);
}

/*
 *  Destructor
 */
appearance::~appearance()
{
    delete ui;
}
