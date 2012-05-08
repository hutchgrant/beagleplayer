#ifndef VOLUME_H
#define VOLUME_H

#include <QWidget>
#include <QPainter>
namespace Ui {
class volume;
}

class volume : public QWidget
{
    Q_OBJECT
    
public:
    explicit volume(QWidget *parent = 0);
    ~volume();
    
private slots:
    void on_volSlider_sliderMoved(int position);

signals:
    void volChanged(int);
protected:
    void paintEvent(QPaintEvent * /* Event */ );

private:
    Ui::volume *ui;

    int curVol;

};

#endif // VOLUME_H
