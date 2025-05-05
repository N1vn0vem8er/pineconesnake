QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alltrackswidget.cpp \
    main.cpp \
    mainwindow.cpp \
    nowplayingwidget.cpp \
    playingwidget.cpp \
    playlistitem.cpp \
    playlistmodel.cpp \
    playlistwidget.cpp \
    resourcesmanager.cpp \
    searchworker.cpp \
    settings.cpp \
    trackitemwidget.cpp \
    tracklistmodel.cpp

HEADERS += \
    alltrackswidget.h \
    mainwindow.h \
    nowplayingwidget.h \
    playingwidget.h \
    playlistitem.h \
    playlistmodel.h \
    playlistwidget.h \
    resourcesmanager.h \
    searchworker.h \
    settings.h \
    structs.h \
    trackitemwidget.h \
    tracklistmodel.h

FORMS += \
    alltrackswidget.ui \
    mainwindow.ui \
    nowplayingwidget.ui \
    playingwidget.ui \
    playlistwidget.ui

TRANSLATIONS += \
    pcsmusicplayer_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

LIBS += -lsqlite3 -ltag

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
