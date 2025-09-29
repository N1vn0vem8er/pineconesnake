QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

SOURCES += \
    src/albumitemwidget.cpp \
    src/albumswidget.cpp \
    src/alltrackswidget.cpp \
    src/artistitemwidget.cpp \
    src/artistswidget.cpp \
    src/favoriteitemwidget.cpp \
    src/favoritemodel.cpp \
    src/favoritewidget.cpp \
    src/fileslistitemwidget.cpp \
    src/fileslistmodel.cpp \
    src/fileswidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/nowplayingwidget.cpp \
    src/playingwidget.cpp \
    src/playlistitem.cpp \
    src/playlistitemwidget.cpp \
    src/playlistmodel.cpp \
    src/playlistnamedialog.cpp \
    src/playlistswidget.cpp \
    src/playlistwidget.cpp \
    src/resourcesmanager.cpp \
    src/searchworker.cpp \
    src/settings.cpp \
    src/settingswidget.cpp \
    src/trackitemwidget.cpp \
    src/tracklistmodel.cpp

HEADERS += \
    src/albumitemwidget.h \
    src/albumswidget.h \
    src/alltrackswidget.h \
    src/artistitemwidget.h \
    src/artistswidget.h \
    src/favoriteitemwidget.h \
    src/favoritemodel.h \
    src/favoritewidget.h \
    src/fileslistitemwidget.h \
    src/fileslistmodel.h \
    src/fileswidget.h \
    src/mainwindow.h \
    src/nowplayingwidget.h \
    src/playingwidget.h \
    src/playlistitem.h \
    src/playlistitemwidget.h \
    src/playlistmodel.h \
    src/playlistnamedialog.h \
    src/playlistswidget.h \
    src/playlistwidget.h \
    src/resourcesmanager.h \
    src/searchworker.h \
    src/settings.h \
    src/settingswidget.h \
    src/structs.h \
    src/trackitemwidget.h \
    src/tracklistmodel.h

FORMS += \
    src/albumswidget.ui \
    src/alltrackswidget.ui \
    src/artistswidget.ui \
    src/favoritewidget.ui \
    src/fileswidget.ui \
    src/mainwindow.ui \
    src/nowplayingwidget.ui \
    src/playingwidget.ui \
    src/playlistnamedialog.ui \
    src/playlistswidget.ui \
    src/playlistwidget.ui \
    src/settingswidget.ui

TRANSLATIONS += \
    src/pcsmusicplayer_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

LIBS += -lsqlite3 -ltag

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
