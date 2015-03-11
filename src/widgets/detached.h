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
    int volume, min, max, state, mode;
    bool songChange, screenMode; // true = full

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
    void remotePage(string page);

    void remoteScreen(bool type){
        this->screenMode = type;
        qDebug() << "remote type set to " << type <<endl;
        emit remScreenToggle(type);
    }


    /*
     * Object setters
     */
    void setTrack(string track, string path, int mode){
        this->songChange = true;
        this->trackName = track;
        this->trackPath = path;
        this->mode = mode;
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
    void setScreenMode(bool fullscreen){
        qDebug() << "setting screenMode " << fullscreen << endl;
        this->screenMode = fullscreen;
    }

    /*
     * Object getters
     */
    QString getPath(){
        return QString(this->trackPath.c_str());
    }
    QString getTrack(){
        return QString(this->trackName.c_str());
    }
    int getMode(){
        return this->mode;
    }
    bool getPlaylistMove(){
        if(this->songChange){
            this->songChange = false;
            return true;
        }
        return false;
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
    bool getScreenMode(){
        qDebug() << "getting screenMode " << screenMode << endl;
        return this->screenMode;
    }

signals:
    void remConRange(int);
    void remConSeek(int);
    void remConFile(int);
    void remConVol(int);
    void remScreenToggle(bool);
    void detachClose();
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
