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
    src/syncall.cpp \
    src/readdb.cpp \
    src/qmpwidget.cpp \
    src/playlist.cpp \
    src/main.cpp \
    src/localsync.cpp \
    src/fileobj.cpp \
    src/controls.cpp \
    src/browse.cpp \
    src/beaglemain.cpp \
    src/preferences.cpp \
    src/prefdialog.cpp

HEADERS  += \
    src/syncall.h \
    src/readdb.h \
    src/qmpyuvreader.h \
    src/qmpwidget.h \
    src/playlist.h \
    src/localsync.h \
    src/fileobj.h \
    src/controls.h \
    src/browse.h \
    src/beaglemain.h \
    src/preferences.h \
    src/prefdialog.h

FORMS    += \
    src/playlist.ui \
    src/controls.ui \
    src/browse.ui \
    src/beaglemain.ui \
    src/prefdialog.ui

OTHER_FILES += \
    qmpwidget.pri \
    src/qmpwidget.pri \
    res/UP_icon.png \
    res/STOP_icon.png \
    res/RWD_icon.png \
    res/RRWD_icon.png \
    res/RPT_icon.png \
    res/PAUSE_icon.png \
    res/FWD_icon.png \
    res/FFWD_icon.png \
    res/DOWN_icon.png \
    res/beagletomb_icon.png

RESOURCES += \
    res/BTres.qrc
