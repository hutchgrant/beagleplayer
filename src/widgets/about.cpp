#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    QPainter paint;

    QGraphicsScene *scn = new QGraphicsScene(ui->graphicsView);
   ui->graphicsView->setScene( scn );

   QPixmap someImage("/home/skynet/Desktop/beagleplayer/res/beagleplayer_icon.png");
   QFont myFont;
   myFont.setBold(true);
   myFont.setFamily("Serif");
   myFont.setPixelSize(20);
   scn->setSceneRect(0,0,0,0);
   scn->addPixmap(someImage);
   ui->graphicsView->show();
}

about::~about()
{
    delete ui;
}
