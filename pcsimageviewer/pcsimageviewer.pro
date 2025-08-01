QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/dialogs/propertiesdialog.cpp \
    src/imageview.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/tabwidget.cpp

HEADERS += \
    src/dialogs/propertiesdialog.h \
    src/imageview.h \
    src/mainwindow.h \
    src/tabwidget.h

FORMS += \
    src/dialogs/propertiesdialog.ui \
    src/mainwindow.ui

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

TRANSLATIONS += \
    pcsimageviewer_pl_PL.ts

CONFIG += lrelease
CONFIG += embed_translations
