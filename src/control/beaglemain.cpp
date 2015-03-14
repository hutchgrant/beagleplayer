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
    delete brow;
    delete playlst;
    delete aBout;
    delete theme;
}

/*
 *  Initialize and add Add each widget to its corresponding layout
 */
void beaglemain::addWidgets(){
   brow = new browse(this);
   playlst = new playlist(this);
   cntrl = new controls(this);
   aBout = new about();
   theme = new appearance();

   ui->browse_layout->addWidget(brow, 0 ,0);
   ui->control_layout->addWidget(cntrl, 0 ,0);
   ui->playlist_layout->addWidget(playlst, 0, 0);
}

/*
 * Initialize the Database and location, once, for all widgets
 */
void beaglemain::initCache(){

    if(dbCache.init()){
        brow->initCache(&dbCache);
        playlst->initCache(&dbCache);
        connect(theme, SIGNAL(themeChanged(string)), cntrl, SLOT(setTheme(string)));
        theme->init(&dbCache);

        brow->Sync(1);  /// initialize and fill DB cache objects
    }
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
      connect(brow, SIGNAL(curListChanged(int, fileObj*, fileObj*,int*,int, bool, int)), cntrl, SLOT(setCurList(int, fileObj*, fileObj*,int*,int, bool, int)));  /// change control's playlist of tracks
      connect(brow,SIGNAL(FullSelection(int)), cntrl, SLOT(setSelectionAndPlay(int)));  /// a full track selection was made (double0click)
      connect(brow,SIGNAL(selectionChanged(int)), cntrl, SLOT(setSelection(int)));  ///  a single track selection was made (single-click)

      /// signal + slots for controlling browsing remotely through controls
      connect(cntrl, SIGNAL(conDirChange(int)), brow, SLOT(changeDirectory(int)));
      connect(cntrl, SIGNAL(conModeChange(int)), brow, SLOT(updateMode(int)));

      /// current selection into playlist
      connect(brow,SIGNAL(trackChanged(string, string, int, int, int)), playlst, SLOT(setTempTrack(string, string, int, int, int)));  ///  a single track selection was made (single-click)
      connect(brow, SIGNAL(startTempTrack(string,string, int)), cntrl, SLOT(setTempTrackAndPlay(string, string, int)));

      /// playlist
      connect(playlst, SIGNAL(playlistChanged(int, fileObj*, fileObj*,int*,int, bool, int)), cntrl, SLOT(setCurList(int, fileObj*, fileObj*,int*,int, bool, int)));
      connect(playlst, SIGNAL(playlistSelection(int)), cntrl, SLOT(setSelection(int)));
      connect(playlst, SIGNAL(playlistFullSelection(int)), cntrl, SLOT(setSelectionAndPlay(int)));

      /// connect toolbar actions
      connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(importAudio()));
      connect(ui->actionImport_Video, SIGNAL(triggered()), this, SLOT(importVideo()));
      connect(ui->actionAdd_Radio_Station, SIGNAL(triggered()), this, SLOT(addRadio()));
      connect(ui->actionOpen_File, SIGNAL(triggered()), this, SLOT(openFile()));
      connect(ui->actionOpen_URL, SIGNAL(triggered()), this, SLOT(openURL()));
      connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutDialog()));
      connect(ui->actionAppearance, SIGNAL(triggered()), this, SLOT(themeDialog()));

      /// refresh and change theme
     connect(theme, SIGNAL(themeChanged(string)), cntrl, SLOT(setTheme(string)));

}


