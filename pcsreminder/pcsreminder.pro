QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

SOURCES += \
    src/createeventdialog.cpp \
    src/editeventdialog.cpp \
    src/eventmanager.cpp \
    src/eventwidget.cpp \
    src/fullscreendialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/repeatedeventwidget.cpp \
    src/resourcesmanager.cpp \
    src/settings.cpp

HEADERS += \
    src/createeventdialog.h \
    src/editeventdialog.h \
    src/eventmanager.h \
    src/eventwidget.h \
    src/fullscreendialog.h \
    src/mainwindow.h \
    src/repeatedeventwidget.h \
    src/resourcesmanager.h \
    src/settings.h

FORMS += \
    src/createeventdialog.ui \
    src/editeventdialog.ui \
    src/eventwidget.ui \
    src/fullscreendialog.ui \
    src/mainwindow.ui \
    src/repeatedeventwidget.ui

TRANSLATIONS += \
    src/pcsreminder_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

LIBS += -lsqlite3

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc
