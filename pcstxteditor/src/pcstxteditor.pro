QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    linenumberarea.cpp \
    main.cpp \
    mainwindow.cpp \
    savewarningdialog.cpp \
    searchwidget.cpp \
    tabwidget.cpp \
    texteditor.cpp \
    texthighlighter.cpp

HEADERS += \
    linenumberarea.h \
    mainwindow.h \
    savewarningdialog.h \
    searchwidget.h \
    tabwidget.h \
    texteditor.h \
    texthighlighter.h

FORMS += \
    mainwindow.ui \
    savewarningdialog.ui \
    searchwidget.ui

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += lrelease
CONFIG += embed_translations

TRANSLATIONS += \
    pcstxteditor_pl_PL.ts
