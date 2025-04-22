QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addnewdialog.cpp \
    editdialog.cpp \
    itemwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    resourcemanager.cpp \
    settings.cpp

HEADERS += \
    addnewdialog.h \
    editdialog.h \
    itemwidget.h \
    mainwindow.h \
    resourcemanager.h \
    settings.h \
    structs.h

FORMS += \
    addnewdialog.ui \
    editdialog.ui \
    itemwidget.ui \
    mainwindow.ui

LIBS += -lsqlite3

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS += \
    pcscontenttracker_pl_PL.ts

CONFIG += lrelease
CONFIG += embed_translations
