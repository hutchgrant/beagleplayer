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

#include "beaglemain.h"
#include "ui_beaglemain.h"

beaglemain::beaglemain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::beaglemain)
{
    ui->setupUi(this);
    addWidgets();
    conSignals();
}

void beaglemain::addWidgets(){
   brow = new browse(this);
   cntrl = new controls(this);
   plList = new playlist(this);
    ui->widgetLayout->addWidget(brow,0,0,0,0,0);
    ui->cntrlWidget->addWidget(cntrl,0,0,0,0,0);
    ui->playwidget->addWidget(plList,0,0,0,0,0);
}

void beaglemain::conSignals(){
    connect(brow, SIGNAL(MenuSelection(int)), brow, SLOT(updateTitle(int)));
    connect(ui->ModeCombo, SIGNAL(currentIndexChanged(int)), brow, SLOT(updateMode(int)));

    //cntrl
    connect(brow, SIGNAL(curListChanged(fileObj&,int*)), cntrl, SLOT(setCurList(fileObj&,int*)));
    connect(brow,SIGNAL(FullSelection(int)), cntrl, SLOT(setSelectionAndPlay(int)));
    connect(brow,SIGNAL(selectionChanged(int)), cntrl, SLOT(setSelection(int)));
}

beaglemain::~beaglemain()
{
    delete ui;
}


void beaglemain::on_actionImport_Video_triggered()
{
    brow->Sync(3);
}

void beaglemain::on_actionImport_triggered()
{
    brow->Sync(2);
}
