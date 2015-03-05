#include "beaglemain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    beaglemain w;
    w.show();
    return a.exec();
}
