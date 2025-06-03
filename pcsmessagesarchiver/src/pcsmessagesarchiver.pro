QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appearancesettingswidget.cpp \
    browsemessageswidget.cpp \
    databasesettingswidget.cpp \
    homewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    managecontactswidget.cpp \
    resourcesmanager.cpp \
    savemessagewidget.cpp \
    settings.cpp \
    settingswidget.cpp \
    writer.cpp

HEADERS += \
    appearancesettingswidget.h \
    browsemessageswidget.h \
    databasesettingswidget.h \
    globals.h \
    homewidget.h \
    mainwindow.h \
    managecontactswidget.h \
    resourcesmanager.h \
    savemessagewidget.h \
    settings.h \
    settingswidget.h \
    writer.h

FORMS += \
    appearancesettingswidget.ui \
    browsemessageswidget.ui \
    databasesettingswidget.ui \
    homewidget.ui \
    mainwindow.ui \
    managecontactswidget.ui \
    savemessagewidget.ui \
    settingswidget.ui \
    writer.ui

LIBS += -lsqlite3

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS += \
    pcsmessagesarchiver_pl_PL.ts

CONFIG += lrelease
CONFIG += embed_translations
