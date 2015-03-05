#include "qweburl.h"
#include "ui_qweburl.h"

QWebUrl::QWebUrl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWebUrl)
{
    ui->setupUi(this);
    url_input = "http://";
    ui->url_input->setText(url_input.c_str());
}

QWebUrl::~QWebUrl()
{
    delete ui;
}

void QWebUrl::on_buttonBox_accepted()
{
    url_input = ui->url_input->text().toStdString();
}
