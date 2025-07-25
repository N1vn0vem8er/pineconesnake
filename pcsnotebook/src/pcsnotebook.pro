QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    allnoteswidget.cpp \
    inputtitledialog.cpp \
    main.cpp \
    mainwindow.cpp \
    notecardwidget.cpp \
    resourcesmanager.cpp \
    settings.cpp \
    writer.cpp \
    linenumberarea.cpp \
    texteditor.cpp

HEADERS += \
    allnoteswidget.h \
    inputtitledialog.h \
    mainwindow.h \
    notecardwidget.h \
    resourcesmanager.h \
    settings.h \
    structs.h \
    writer.h \
    linenumberarea.h \
    texteditor.h

FORMS += \
    allnoteswidget.ui \
    inputtitledialog.ui \
    mainwindow.ui \
    notecardwidget.ui \
    writer.ui

LIBS += -lsqlite3

TRANSLATIONS += \
    pcsnotebook_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
