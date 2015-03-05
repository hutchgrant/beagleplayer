#include "newplaylist.h"
#include "ui_newplaylist.h"

/*
 * Constructor
 */
newplaylist::newplaylist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newplaylist)
{

    plName = "-";
    ui->setupUi(this);
}

/*
 * Destructor
 */
newplaylist::~newplaylist()
{
    delete ui;
}

/*
 * Store the playlist name, when okay button pressed
 */
void newplaylist::on_playlist_accept_accepted()
{
    plName = ui->playlist_entry->text().toStdString();
}
