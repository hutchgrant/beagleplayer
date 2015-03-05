#include "beaglemain.h"
#include "ui_beaglemain.h"

/*
 * Constructor
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
 * Destructor
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
   aBout = new about(this);
   detached = new skincntrl();
   detached->hide();

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
 *  Update Mode_Change widget at top with any given mode
 */
void beaglemain::changeMode(int mode){
    ui->Mode_Change->setCurrentIndex(mode);
}

/*
 *  Connect Signals for widgets
 */
void beaglemain::connectSignals(){
      /// Browse  + control base signals
      connect(ui->Mode_Change, SIGNAL(currentIndexChanged(int)), brow, SLOT(updateMode(int)));   /// change from audio/video mode
      connect(brow, SIGNAL(curListChanged(fileObj&,int*,int)), cntrl, SLOT(setCurList(fileObj&,int*,int)));  /// change control's playlist of tracks
      connect(brow,SIGNAL(FullSelection(int)), cntrl, SLOT(setSelectionAndPlay(int)));  /// a full track selection was made (double0click)
      connect(brow,SIGNAL(selectionChanged(int)), cntrl, SLOT(setSelection(int)));  ///  a single track selection was made (single-click)

      /// current selection into playlist
      connect(brow,SIGNAL(trackChanged(string, string, int, int)), playlst, SLOT(setTempTrack(string, string, int, int)));  ///  a single track selection was made (single-click)
      connect(brow, SIGNAL(startTempTrack(string,string)), cntrl, SLOT(setTempTrackAndPlay(string, string)));

      /// playlist
      connect(playlst, SIGNAL(playlistChanged(fileObj&,int*,int)), cntrl, SLOT(setCurList(fileObj&,int*,int)));
      connect(playlst, SIGNAL(playlistSelection(int)), cntrl, SLOT(setSelection(int)));
      connect(playlst, SIGNAL(playlistFullSelection(int)), cntrl, SLOT(setSelectionAndPlay(int)));

      /// connect controls to detached player
      connect(cntrl, SIGNAL(detach()), this, SLOT(detachControls()) );
      connect(cntrl, SIGNAL(songChanged(string)), detached, SLOT(setTrack(string)) );
      connect(cntrl, SIGNAL(remConSeek(int)), detached, SLOT(setSeekPos(int)) );
      connect(cntrl, SIGNAL(remConRange(int)), detached, SLOT(setSeekRange(int)));
      connect(cntrl, SIGNAL(remConVol(int)), detached, SLOT(setMainVol(int)) );

      /// connect detach player to controls
      connect(detached, SIGNAL(remConSeek(int)), cntrl, SLOT(remoteSeek(int)));
      connect(detached, SIGNAL(remConFile(int)), cntrl, SLOT(remoteCommand(int)));
      connect(detached, SIGNAL(remConVol(int)), cntrl, SLOT(remoteVolume(int)));

      /// connect toolbar actions
      connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(importAudio()));
      connect(ui->actionImport_Video, SIGNAL(triggered()), this, SLOT(importVideo()));
      connect(ui->actionAdd_Radio_Station, SIGNAL(triggered()), this, SLOT(addRadio()));
      connect(ui->actionOpen_File, SIGNAL(triggered()), this, SLOT(openFile()));
      connect(ui->actionOpen_URL, SIGNAL(triggered()), this, SLOT(openURL()));
      connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutDialog()));
}


