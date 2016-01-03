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
    themes.initFile(100);
    selectTheme = 0;
    defaultTheme = 0;
}

/*
 *  Get all themes from ./themes folder
 */
void appearance::getThemes(){
    themes = fileObj();
    themes.initFile(100);
    int themeCount = 0;
    int count = 0;
    fileObj themeDir;
    themeDir.initFile(100);
    QString theme = QApplication::applicationDirPath() + "/res/themes/";
    QDirIterator directories(theme.toStdString().c_str(), QDir::Dirs | QDir::NoDotAndDotDot);
    while(directories.hasNext()){
        directories.next();
        themeDir.set(count, count, 0, directories.fileName().toStdString().c_str(), directories.filePath().toStdString().c_str());
        count++;
    }

       for(int x=0; x< themeDir.getSize(); x++){
        QDirIterator dirWalk(QString::fromStdString(themeDir.getPath(x)), QDir::Files | QDir::NoSymLinks);
        while(dirWalk.hasNext())
        {
           dirWalk.next();
           if(dirWalk.fileInfo().completeSuffix() == "theme"){
              themes.set(themeCount,themeCount,0,dirWalk.fileName().toStdString().c_str(), dirWalk.filePath().toStdString().c_str());
              themeCount++;
           }
        }
       }
}

/*
 *  Set the selected theme's image to the appearance display
 */
void appearance::setThemeImg(string imgPath){

    int lastDot = QString(imgPath.c_str()).lastIndexOf(".");
    QString subImgPath = QString(imgPath.c_str()).mid(0, lastDot);
    subImgPath = subImgPath.append(".png");

    scn = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene( scn );
    themeImg = QPixmap(subImgPath);
    scn->setSceneRect(0,0,0,0);
    scn->addPixmap(themeImg);
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
        ui->theme_selected->setText(strings.at(0));
        ui->theme_author->setText(strings.at(2));
        ui->theme_version->setText(strings.at(3));
        ui->theme_title->setText(strings.at(1));
        ui->theme_date->setText(strings.at(4));
        ui->theme_desc->setText(strings.at(5));
    }

}

/*
 *  Fill Themes in list
 */
void appearance::fillThemes(){
    QStringList themeMenu;
    t_Model = new QStringListModel(this);
    for(int i=0; i <= themes.getSize(); i++){
            themeMenu << themes.getName(i);
     }
    t_Model->setStringList(themeMenu);
    ui->theme_list->setModel(t_Model);
}

/*
 *  Signal when theme list is clicked
 */
void appearance::on_theme_list_clicked(const QModelIndex &index)
{
    selectTheme = ui->theme_list->currentIndex().row();
    parseTheme(themes.getPath(selectTheme));
    setThemeImg(themes.getPath(selectTheme));
}

/*
 *  Signal when appearance window Apply || Reset || Cancel is selected
 */
void appearance::on_buttonBox_clicked(QAbstractButton *button)
{
    string buttonText = "";
    buttonText = button->text().toStdString();
    if(buttonText == "Reset"){
        reset();
    }else if(buttonText == "Apply"){
        apply(themes.getName(selectTheme), themes.getPath(selectTheme));
        ui->theme_current->setText(themes.getName(selectTheme));
    }
}

/*
 *  Reset appearance to default theme
 */
void appearance::reset(){
    apply(themes.getName(defaultTheme), themes.getPath(defaultTheme));  // set to default index
}

/*
 *  Apply selected theme, storing its path and name in local db cache
 */
void appearance::apply(string file, string path){
    fileObj addTheme;
    addTheme.initFile(100);
    addTheme.set(0,0,0,file.c_str(), path.c_str());
    cah->updateDB(&addTheme, "theme");
    this->hide();
    emit themeChanged(path.c_str());
}

/*
 *  Set Current selected theme, to the theme stored in local cache
 */
void appearance::setCurrent(){
    string path = "";
    cah->readDB(themes, "theme");
    if(themes.getSize() != 0){
        path = themes.getPath(defaultTheme);
        emit themeChanged(path);
    }
}

/*
 * Add the default theme to the local cache
 */
bool appearance::createDefaultTheme(){
    fileObj addTheme;
    addTheme.initFile(100);
    QString theme = QApplication::applicationDirPath() + "/res/themes/default/default.theme";
    addTheme.set(0,0,0,"default", theme.toStdString().c_str());
    if(cah->writeDB(&addTheme, "theme") >=0){
        return true;
    }
    return false;
}

/*
 *  Destructor
 */
appearance::~appearance()
{
    delete ui;
}
