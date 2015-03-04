#ifndef BEAGLEMAIN_H
#define BEAGLEMAIN_H

#include <QMainWindow>
#include "src/widgets/browse.h"
#include "src/cache/cache.h"
#include "src/widgets/playlist.h"
#include "src/widgets/controls.h"
#include "src/widgets/skincntrl.h"

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

private slots:

    void on_actionImport_triggered();

    void on_actionImport_Video_triggered();

    void on_actionAdd_Radio_Station_triggered();

    void detachControls(){
        detached->show();
    }

private:
    Ui::beaglemain *ui;
    browse *brow;
    browse *brow2;
    browse *brow3;

    playlist *playlst;
    controls *cntrl;
    skincntrl *detached;

};

#endif // BEAGLEMAIN_H
