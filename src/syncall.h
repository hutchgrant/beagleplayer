#ifndef SYNCALL_H
#define SYNCALL_H
#include "localsync.h"
#include "readdb.h"
#include <stdlib.h>
#include <stdio.h>
#include <QWidget>
#include <QFileDialog>
#include "preferences.h"
#include "prefdialog.h"
namespace Ui
{
    class syncAll;
}
class syncAll : public QMainWindow
{
    Q_OBJECT

public:
    localsync lclSync;
    preferences pref;
    PrefDialog prefDg;
    readDB rDB;
    syncAll();

    void fillObjects(fileObj &Artist, fileObj &Song, fileObj &VidDir, fileObj &Video);
    void control(fileObj &Artist, fileObj &Song, fileObj &VidDir, fileObj &Video, int rwmode);
    void setPref();
    void dispPref();
};

#endif // SYNCALL_H
