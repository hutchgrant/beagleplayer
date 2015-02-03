#-------------------------------------------------
#
# Project created by QtCreator 2015-01-06T02:41:46
#
#-------------------------------------------------

QT       += core gui sql network opengl

TARGET = beagleplayer
TEMPLATE = app


SOURCES += src/control/main.cpp\
        src/control/beaglemain.cpp \
    src/widgets/browse.cpp \
    src/cache/cache.cpp \
    src/widgets/playlist.cpp \
    src/widgets/newplaylist.cpp \
    src/object/fileobj.cpp \
    src/widgets/controls.cpp \
    src/widgets/volume.cpp \
    src/qmpwidget/qmpwidget.cpp \
    src/cache/localsync.cpp \
    src/widgets/radiostat.cpp

HEADERS  += src/control/beaglemain.h \
    src/widgets/browse.h \
    src/cache/cache.h \
    src/widgets/playlist.h \
    src/widgets/newplaylist.h \
    src/object/fileobj.h \
    src/widgets/controls.h \
    src/widgets/volume.h \
    src/qmpwidget/qmpwidget.h \
    src/qmpwidget/qmpyuvreader.h \
    src/cache/localsync.h \
    src/widgets/radiostat.h

FORMS    += src/control/beaglemain.ui \
    src/widgets/browse.ui \
    src/widgets/playlist.ui \
    src/widgets/newplaylist.ui \
    src/widgets/volume.ui \
    src/widgets/controls.ui \
    src/widgets/radiostat.ui

OTHER_FILES += \
    src/qmpwidget/qmpwidget.pri

RESOURCES += \
    res/BPres.qrc
