#ifndef APPEARANCE_H
#define APPEARANCE_H

#include <QDialog>
#include <QFileDialog>
#include "src/cache/cache.h"

namespace Ui {
class appearance;
}

class appearance : public QDialog
{
    Q_OBJECT

public:
    explicit appearance(QWidget *parent = 0);
    ~appearance();
    void getThemes();
    void fillThemes();
    void parseTheme(string filename);
    void setThemeImg(string imgPath);
public slots:

    void init(cache *dbcon){
        string path = "";
        cah = dbcon;
        getThemes();
        fillThemes();
        path = themes->getPath(0);
        parseTheme(path);
        path = path.replace(path.end()-5, path.end(),"png");
        setThemeImg(path);
    }

private:
    Ui::appearance *ui;
    cache *cah;
    fileObj *themes;
    QStringListModel *t_Model;

};

#endif // APPEARANCE_H
