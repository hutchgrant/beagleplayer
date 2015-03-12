#ifndef APPEARANCE_H
#define APPEARANCE_H

#include <QDialog>
#include <QFileDialog>
#include <QAbstractButton>
#include <QGraphicsScene>
#include "src/cache/cache.h"

namespace Ui {
class appearance;
}

class appearance : public QDialog
{
    Q_OBJECT

public:
    int selectTheme, defaultTheme;
    cache *cah;

    explicit appearance(QWidget *parent = 0);
    ~appearance();
    void getThemes();
    void fillThemes();
    void parseTheme(string filename);
    void setThemeImg(string imgPath);

    bool createDefaultTheme();
    void reset();
    void apply(string file, string path);
    void setCurrent();

public slots:

    void init(cache *dbcon){
        cah = dbcon;
        setCurrent();
    }

private slots:
    void on_theme_list_clicked(const QModelIndex &index);
    void on_buttonBox_clicked(QAbstractButton *button);

signals:
    void themeChanged(string);

private:
    Ui::appearance *ui;
    fileObj themes;
    QStringListModel *t_Model;
};

#endif // APPEARANCE_H
