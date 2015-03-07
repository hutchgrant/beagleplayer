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

    string trackName;
    int volume, min, max, state;

    explicit detached(QWidget *parent = 0);
    virtual ~detached();

private slots:
    void addToJavaScript();
public slots:

    void setTrack(string track){
        qDebug() << "track set = " << track.c_str() << endl;
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
