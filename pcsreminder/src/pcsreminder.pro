QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    createeventdialog.cpp \
    editeventdialog.cpp \
    eventmanager.cpp \
    eventwidget.cpp \
    fullscreendialog.cpp \
    main.cpp \
    mainwindow.cpp \
    repeatedeventwidget.cpp \
    resourcesmanager.cpp \
    settings.cpp

HEADERS += \
    createeventdialog.h \
    editeventdialog.h \
    eventmanager.h \
    eventwidget.h \
    fullscreendialog.h \
    mainwindow.h \
    repeatedeventwidget.h \
    resourcesmanager.h \
    settings.h

FORMS += \
    createeventdialog.ui \
    editeventdialog.ui \
    eventwidget.ui \
    fullscreendialog.ui \
    mainwindow.ui \
    repeatedeventwidget.ui

TRANSLATIONS += \
    pcsreminder_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

LIBS += -lsqlite3

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc
