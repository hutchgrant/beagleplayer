#-------------------------------------------------
#
# Project created by QtCreator 2012-05-07T21:07:53
#
#-------------------------------------------------

QT       += core gui sql

TARGET = beagleplayer
TEMPLATE = app
QT += network \
        opengl

SOURCES += \
    src/qmpwidget.cpp \
    src/playlist.cpp \
    src/main.cpp \
    src/localsync.cpp \
    src/fileobj.cpp \
    src/controls.cpp \
    src/browse.cpp \
    src/beaglemain.cpp \
    src/prefdialog.cpp \
    src/about.cpp \
    src/volume.cpp \
    src/dbconnect.cpp \
    src/newplaylist.cpp

HEADERS  += \
    src/qmpyuvreader.h \
    src/qmpwidget.h \
    src/playlist.h \
    src/localsync.h \
    src/fileobj.h \
    src/controls.h \
    src/browse.h \
    src/beaglemain.h \
    src/prefdialog.h \
    src/about.h \
    src/volume.h \
    src/dbconnect.h \
    src/newplaylist.h

FORMS    += \
    src/playlist.ui \
    src/controls.ui \
    src/browse.ui \
    src/beaglemain.ui \
    src/prefdialog.ui \
    src/about.ui \
    src/volume.ui \
    src/newplaylist.ui

OTHER_FILES += \
    qmpwidget.pri \
    src/qmpwidget.pri \
    images/UP_icon.png \
    images/STOP_icon.png \
    images/RWD_icon.png \
    images/RRWD_icon.png \
    images/RPT_icon.png \
    images/PAUSE_icon.png \
    images/FWD_icon.png \
    images/FFWD_icon.png \
    images/DOWN_icon.png \
    images/beagleplayer_icon.png \
    images/REMOVE_icon.png \
    images/OPEN_icon.png \
    images/ADD_icon.png

RESOURCES += \
    images/BTres.qrc

documentation.files = docs/*
documentation.path = /opt/extras.ubuntu.com/beagleplayer/docs
sources.files = src/*
sources.path = /opt/extras.ubuntu.com/beagleplayer/src
target.path = /opt/extras.ubuntu.com/beagleplayer
INSTALLS += target \
            sources \
            documentation 
