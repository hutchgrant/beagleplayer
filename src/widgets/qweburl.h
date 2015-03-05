#ifndef QWEBURL_H
#define QWEBURL_H

#include <QDialog>

using namespace std;
namespace Ui {
class QWebUrl;
}

class QWebUrl : public QDialog
{
    Q_OBJECT

public:
    string url_input;
    explicit QWebUrl(QWidget *parent = 0);
    ~QWebUrl();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::QWebUrl *ui;
};

#endif // QWEBURL_H
