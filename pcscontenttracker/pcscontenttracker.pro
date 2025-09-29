QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/addnewdialog.cpp \
    src/editdialog.cpp \
    src/itemwidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/resourcemanager.cpp \
    src/settings.cpp

HEADERS += \
    src/addnewdialog.h \
    src/editdialog.h \
    src/itemwidget.h \
    src/mainwindow.h \
    src/resourcemanager.h \
    src/settings.h \
    src/structs.h

FORMS += \
    src/addnewdialog.ui \
    src/editdialog.ui \
    src/itemwidget.ui \
    src/mainwindow.ui

LIBS += -lsqlite3

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS += \
    src/pcscontenttracker_pl_PL.ts

CONFIG += lrelease
CONFIG += embed_translations

RESOURCES += \
    resources/resources.qrc
