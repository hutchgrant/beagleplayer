#include "beaglemain.h"
#include "ui_beaglemain.h"

/*
 * Construct
 */
beaglemain::beaglemain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::beaglemain)
{
    ui->setupUi(this);
    addWidgets();
    initCache();
    connectSignals();
    ui->menuBar->show();
}

/*
 * Deconstruct
 */
beaglemain::~beaglemain()
{
    delete ui;
}

/*
 *  Initialize and add Add each widget to its corresponding layout
 */
void beaglemain::addWidgets(){
   brow = new browse(this);
   playlst = new playlist(this);
   cntrl = new controls(this);

   ui->browse_layout->addWidget(brow, 0 ,0);
   ui->control_layout->addWidget(cntrl, 0 ,0);
   ui->playlist_layout->addWidget(playlst, 0, 0);
}

/*
 * Initialize the Database and location, once, for all widgets
 */
void beaglemain::initCache(){
    dbCache.init();  /// determine db location, if exists, if not create it

    brow->initCache(&dbCache);
    playlst->initCache(&dbCache);

    brow->Sync(1);  /// initialize and fill DB cache objects
}

/*
 *  Connect Signals for widgets
 */
void beaglemain::connectSignals(){
      connect(ui->Mode_Change, SIGNAL(currentIndexChanged(int)), brow, SLOT(updateMode(int)));   /// change from audio/video mode
      connect(brow, SIGNAL(curListChanged(fileObj&,int*)), cntrl, SLOT(setCurList(fileObj&,int*)));  /// change control's playlist of tracks
      connect(brow,SIGNAL(FullSelection(int)), cntrl, SLOT(setSelectionAndPlay(int)));  /// a full track selection was made (double0click)
      connect(brow,SIGNAL(selectionChanged(int)), cntrl, SLOT(setSelection(int)));  ///  a single track selection was made (single-click)

      /// connect playlist signals
      connect(brow,SIGNAL(playModeChanged(string)), playlst, SLOT(setPlaylistMode(string)));  ///  playlist list mode changed e.g. radio/playlist/newlist
}

/*
 * Main Tool Bar, File-> Import Audio
 */
void beaglemain::on_actionImport_triggered()
{
    brow->Sync(2);  /// import video dialog
}

/*
 * Main Tool Bar, File-> Import Video
 */
void beaglemain::on_actionImport_Video_triggered()
{
    brow->Sync(3); /// import audio dialog
}

/*
 * Main Tool Bar, Options-> Add Radio Station
 */
void beaglemain::on_actionAdd_Radio_Station_triggered()
{
    brow->Sync(4);
}
