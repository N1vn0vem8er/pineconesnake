QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

SOURCES += \
    src/allnoteswidget.cpp \
    src/inputtitledialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/notecardwidget.cpp \
    src/resourcesmanager.cpp \
    src/settings.cpp \
    src/writer.cpp \
    src/linenumberarea.cpp \
    src/texteditor.cpp

HEADERS += \
    src/allnoteswidget.h \
    src/inputtitledialog.h \
    src/mainwindow.h \
    src/notecardwidget.h \
    src/resourcesmanager.h \
    src/settings.h \
    src/structs.h \
    src/writer.h \
    src/linenumberarea.h \
    src/texteditor.h

FORMS += \
    src/allnoteswidget.ui \
    src/inputtitledialog.ui \
    src/mainwindow.ui \
    src/notecardwidget.ui \
    src/writer.ui

LIBS += -lsqlite3

TRANSLATIONS += \
    src/pcsnotebook_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target
