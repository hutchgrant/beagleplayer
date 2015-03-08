/*
 *  Written By: Grant Hutchinson
 *  License: GPLv3.
 *  h.g.utchinson@gmail.com
 *  Copyright (C) 2012 by Grant Hutchinson
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "detached.h"
#include <QtNetwork>

detached::detached(QWidget *parent) : Html5ApplicationViewer(parent)
{
    trackName = "";
    volume = 0;
    min = 0;
    max = 0;
    state = 0;

    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    connect(webView()->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(addToJavaScript()));
}

void detached::addToJavaScript() {
    webView()->page()->mainFrame()->addToJavaScriptWindowObject("detached", this);
}

detached::~detached(){

}
