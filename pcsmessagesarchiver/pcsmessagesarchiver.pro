QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

SOURCES += \
    src/appearancesettingswidget.cpp \
    src/browsemessageswidget.cpp \
    src/databasesettingswidget.cpp \
    src/homewidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/managecontactswidget.cpp \
    src/resourcesmanager.cpp \
    src/savemessagewidget.cpp \
    src/settings.cpp \
    src/settingswidget.cpp \
    src/writer.cpp

HEADERS += \
    src/appearancesettingswidget.h \
    src/browsemessageswidget.h \
    src/databasesettingswidget.h \
    src/globals.h \
    src/homewidget.h \
    src/mainwindow.h \
    src/managecontactswidget.h \
    src/resourcesmanager.h \
    src/savemessagewidget.h \
    src/settings.h \
    src/settingswidget.h \
    src/writer.h

FORMS += \
    src/appearancesettingswidget.ui \
    src/browsemessageswidget.ui \
    src/databasesettingswidget.ui \
    src/homewidget.ui \
    src/mainwindow.ui \
    src/managecontactswidget.ui \
    src/savemessagewidget.ui \
    src/settingswidget.ui \
    src/writer.ui

LIBS += -lsqlite3

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS += \
    src/pcsmessagesarchiver_pl_PL.ts

CONFIG += lrelease
CONFIG += embed_translations
