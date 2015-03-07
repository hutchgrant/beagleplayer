#include "detached.h"
#include <QtNetwork>

detached::detached(QWidget *parent) : Html5ApplicationViewer(parent)
{
    trackName = "";
    volume = 0;
    min = 0;
    max = 0;
    state = 0;
    QFile file;
    file.setFileName(":/js/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    file.close();

    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    QObject::connect(webView()->page()->mainFrame(),
            SIGNAL(javaScriptWindowObjectCleared()), SLOT(addToJavaScript()));
    QObject::connect(webView()->page()->mainFrame(),
            SIGNAL(loadFinished(bool)), SLOT(addJquery(bool)));

}

void detached::addToJavaScript() {
    webView()->page()->mainFrame()->addToJavaScriptWindowObject("detached", this);
}

detached::~detached(){

}
