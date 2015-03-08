#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDebug>
#include <QGraphicsWebView>
#include <QWebFrame>
#include "html5applicationviewer.h"

using namespace std;
class detached : public Html5ApplicationViewer
{
    Q_OBJECT

public:

    QString jQueryUI = "", jQuery="";
    string trackName;
    int volume, min, max, state;

    explicit detached(QWidget *parent = 0);
    virtual ~detached();

private slots:
    void addToJavaScript();
    void addJquery(bool add);
public slots:
    /* Commands to remotely signal player controls
     * connect = 1  - play
     * connect = 2 - pause
     * connect = 3 - stop
     * connect = 4 - next
     * connect = 5 - prev
     */
    void remoteCmd(int cmd){
        emit remConFile(cmd);
    }
    void remoteVol(int vol){
        this->volume = vol;
        emit remConVol(vol);
    }
    void remoteSeek(int pos){
        this->min = pos;
        emit remConSeek(pos);
    }

    void setTrack(string track){
        this->trackName = track;
    }
    void setSeekPos(int pos){
        this->min = pos;
    }
    void setVolume(int vol){
        this->volume = vol;
    }
    void setRange(int end){
        this->max = end;
    }
    void setState(int mediaState){
        this->state = mediaState;
    }

    QString getTrack(){
        return QString(trackName.c_str());
    }
    int getVolume(){
        return this->volume;
    }
    int getRange(){
        return this->max;
    }
    int getSeekPos(){
        return this->min;
    }

signals:
    void remConSeek(int);
    void remConFile(int);
    void remConVol(int);
};

#endif // MAINWINDOW_H
