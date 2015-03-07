#include "detached.h"

detached::detached(QWidget *parent) : Html5ApplicationViewer(parent)
{
    trackName = "";
    volume = 0;
    min = 0;
    max = 0;
    state = 0;

    QObject::connect(webView()->page()->mainFrame(),
            SIGNAL(javaScriptWindowObjectCleared()), SLOT(addToJavaScript()));
}

void detached::addToJavaScript() {
    webView()->page()->mainFrame()->addToJavaScriptWindowObject("detached", this);
}

detached::~detached(){

}
