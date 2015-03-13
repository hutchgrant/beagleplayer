#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDebug>
#include <QGraphicsWebView>
#include <QWebFrame>
#include "html5applicationviewer.h"
#include "src/object/fileobj.h"

using namespace std;
class detached : public Html5ApplicationViewer
{
    Q_OBJECT

public:
    explicit detached(QWidget *parent = 0);
    virtual ~detached();
    void displayHistory();
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
    void remotePage(QString page);

    void remoteScreen(bool type){
        this->screenMode = type;
        emit remScreenToggle(type);
    }
    void remoteTrack(int track){
        emit remTrackChange(track);
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
        this->screenMode = fullscreen;
    }
    void setTheme(string theme){
        this->wTheme = theme;
        this->wPath = theme.substr(0, theme.find_last_of("/")+1);
    }
    void setCurList(int selID, fileObj *newlist, int * newIDlist,int amt, bool range, int mode){
        this->current = newlist;
        this->curList = newIDlist;
        this->curAmount = amt;
        this->curRange = range;
        this->mode = mode;
        this->directoryID = selID;
    }
    void setSelection(int select){
        this->selectedID = select;
    }
    void setDirSelection(int select){
        this->directoryID = select;
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
        return this->screenMode;
    }
    int getSelectedID(){
       return this->selectedID;
    }
    int getSelectedDirID(){
       return this->directoryID;
    }


signals:
    void remConRange(int);
    void remConSeek(int);
    void remConFile(int);
    void remConVol(int);
    void remScreenToggle(bool);
    void remTrackChange(int);
    void detachClose();
protected:
    void closeEvent(QCloseEvent *event);
private:
    int *curList;       /// current cue list ID's
    int curAmount; /// init for playlist size
    bool curRange;

    string trackName, trackPath, wTheme, wPath;
    int volume, min, max, state, mode, directoryID, selectedID;
    bool songChange, screenMode; // true = full

    fileObj *current;
};

#endif // MAINWINDOW_H
