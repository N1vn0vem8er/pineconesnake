QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

SOURCES += \
    src/linenumberarea.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/savewarningdialog.cpp \
    src/searchwidget.cpp \
    src/tabwidget.cpp \
    src/texteditor.cpp \
    src/texthighlighter.cpp

HEADERS += \
    src/linenumberarea.h \
    src/mainwindow.h \
    src/savewarningdialog.h \
    src/searchwidget.h \
    src/tabwidget.h \
    src/texteditor.h \
    src/texthighlighter.h

FORMS += \
    src/mainwindow.ui \
    src/savewarningdialog.ui \
    src/searchwidget.ui

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += lrelease
CONFIG += embed_translations

TRANSLATIONS += \
    src/pcstxteditor_pl_PL.ts
