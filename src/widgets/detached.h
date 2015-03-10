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

    string trackName, trackPath;
    int volume, min, max, state;
    bool songChange;

    explicit detached(QWidget *parent = 0);
    virtual ~detached();

private slots:
    void addToJavaScript();
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
    void remoteRange(int max){
        emit remConRange(max);
    }

    void setTrack(string track, string path){
        this->songChange = true;
        this->trackName = track;
        this->trackPath = path;
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

    QString getPath(){
        return QString(this->trackPath.c_str());
    }

    QString getTrack(){
        return QString(trackName.c_str());
    }
    bool getPlaylistMove(){
        return this->songChange;
        if(this->songChange){
            this->songChange = false;
        }
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
    int getState(){
        return this->state;
    }

signals:
    void remConRange(int);
    void remConSeek(int);
    void remConFile(int);
    void remConVol(int);
};

#endif // MAINWINDOW_H
