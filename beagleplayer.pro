#-------------------------------------------------
#
# Project created by QtCreator 2015-01-06T02:41:46
#
#-------------------------------------------------

QT       += core gui sql network widgets

TARGET = beagleplayer
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4):QT += widgets webkitwidgets

# Add more folders to ship with the application, here
 folder_01.source = res/themes
 folder_01.target = ./res
 DEPLOYMENTFOLDERS = folder_01

# Define TOUCH_OPTIMIZED_NAVIGATION for touch optimization and flicking
#DEFINES += TOUCH_OPTIMIZED_NAVIGATION

# Please do not modify the following two lines. Required for deployment.
include(src/html5applicationviewer/html5applicationviewer.pri)
qtcAddDeployment()


SOURCES += src/control/main.cpp\
        src/control/beaglemain.cpp \
    src/widgets/browse.cpp \
    src/cache/cache.cpp \
    src/widgets/playlist.cpp \
    src/widgets/newplaylist.cpp \
    src/object/fileobj.cpp \
    src/widgets/controls.cpp \
    src/widgets/volume.cpp \
    src/cache/localsync.cpp \
    src/widgets/radiostat.cpp \
    src/widgets/about.cpp \
    src/widgets/qweburl.cpp \
    src/widgets/detached.cpp \
    src/widgets/appearance.cpp

HEADERS  += src/control/beaglemain.h \
    src/widgets/browse.h \
    src/cache/cache.h \
    src/widgets/playlist.h \
    src/widgets/newplaylist.h \
    src/object/fileobj.h \
    src/widgets/controls.h \
    src/widgets/volume.h \
    src/cache/localsync.h \
    src/widgets/radiostat.h \
    src/widgets/about.h \
    src/widgets/qweburl.h \
    src/widgets/detached.h \
    src/widgets/appearance.h

FORMS    += src/control/beaglemain.ui \
    src/widgets/browse.ui \
    src/widgets/newplaylist.ui \
    src/widgets/volume.ui \
    src/widgets/controls.ui \
    src/widgets/radiostat.ui \
    src/widgets/about.ui \
    src/widgets/qweburl.ui \
    src/widgets/appearance.ui \
    src/widgets/playlist.ui

RESOURCES += \
    res/BPres.qrc

documentation.files = docs/*
linux:documentation.path = /opt/extras.ubuntu.com/beagleplayer/docs
win32:documentation.path = c:\Progra~1\beagleplayer\docs

sources.files = src/*
linux:sources.path = /opt/extras.ubuntu.com/beagleplayer/src
win32:sources.path = c:\Progra~1\beagleplayer\src

linux:target.path = /opt/extras.ubuntu.com/beagleplayer
win32:target.path = c:\Progra~1\beagleplayer

other.files = ./*
linux:other.path = /opt/extras.ubuntu.com/beagleplayer/release
win32:other.path = $$(HOMEDRIVE)\$$(HOMEPATH)\beagleplayer

res.files = res/*
linux:res.path = /opt/extras.ubuntu.com/beagleplayer/res
win32:res.path = $$(HOMEDRIVE)\$$(HOMEPATH)\beagleplayer\res

INSTALLS += target \
            sources \
            documentation \
            other \
            res
