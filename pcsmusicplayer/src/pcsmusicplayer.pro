QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    albumitemwidget.cpp \
    albumswidget.cpp \
    alltrackswidget.cpp \
    artistitemwidget.cpp \
    artistswidget.cpp \
    favoriteitemwidget.cpp \
    favoritemodel.cpp \
    favoritewidget.cpp \
    fileslistitemwidget.cpp \
    fileslistmodel.cpp \
    fileswidget.cpp \
    main.cpp \
    mainwindow.cpp \
    nowplayingwidget.cpp \
    playingwidget.cpp \
    playlistitem.cpp \
    playlistmodel.cpp \
    playlistswidget.cpp \
    playlistwidget.cpp \
    resourcesmanager.cpp \
    searchworker.cpp \
    settings.cpp \
    trackitemwidget.cpp \
    tracklistmodel.cpp

HEADERS += \
    albumitemwidget.h \
    albumswidget.h \
    alltrackswidget.h \
    artistitemwidget.h \
    artistswidget.h \
    favoriteitemwidget.h \
    favoritemodel.h \
    favoritewidget.h \
    fileslistitemwidget.h \
    fileslistmodel.h \
    fileswidget.h \
    mainwindow.h \
    nowplayingwidget.h \
    playingwidget.h \
    playlistitem.h \
    playlistmodel.h \
    playlistswidget.h \
    playlistwidget.h \
    resourcesmanager.h \
    searchworker.h \
    settings.h \
    structs.h \
    trackitemwidget.h \
    tracklistmodel.h

FORMS += \
    albumswidget.ui \
    alltrackswidget.ui \
    artistswidget.ui \
    favoritewidget.ui \
    fileswidget.ui \
    mainwindow.ui \
    nowplayingwidget.ui \
    playingwidget.ui \
    playlistswidget.ui \
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
