#ifndef BEAGLEMAIN_H
#define BEAGLEMAIN_H

#include <QMainWindow>
#include "src/widgets/browse.h"
#include "src/cache/cache.h"
#include "src/widgets/playlist.h"
#include "src/widgets/controls.h"
#include "src/widgets/skincntrl.h"
#include "src/widgets/about.h"
#include "src/widgets/appearance.h"

namespace Ui {
class beaglemain;
}

class beaglemain : public QMainWindow
{
    Q_OBJECT

public:
    cache dbCache;

    explicit beaglemain(QWidget *parent = 0);
    void addWidgets();
    virtual ~beaglemain();

    void initCache();
    void connectSignals();
    void changeMode(int mode);
private slots:

    void detachControls(){
        detached->show();
    }
    /*
     * Main Tool Bar, File-> Import Audio
     */
    void importAudio(){
        changeMode(0);
        brow->Sync(2);  /// import video dialog
    }
    void importVideo(){
        changeMode(1);
        brow->Sync(3); /// import audio dialog
    }
    void openFile(){
        brow->Sync(0);
    }
    void openURL(){
        brow->Sync(-1);
    }
    void addRadio()
    {
        changeMode(2);
        brow->Sync(4);
    }
    void aboutDialog(){
        aBout->show();
    }
    void themeDialog(){
        theme->init(&dbCache);
        theme->show();
    }

private:
    Ui::beaglemain *ui;
    browse *brow;
    browse *brow2;
    browse *brow3;

    playlist *playlst;
    controls *cntrl;
    skincntrl *detached;
    about *aBout;
    appearance *theme;
};

#endif // BEAGLEMAIN_H
