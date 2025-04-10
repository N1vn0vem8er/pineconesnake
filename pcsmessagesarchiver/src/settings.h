#ifndef SETTINGS_H
#define SETTINGS_H

#include "qdir.h"
#include <QString>


class Settings
{
public:
    Settings();
    static QString databasePath;
    static QString theme;
    static QString databaseName;
    static int mainSplitterStretchFactor;
    static int browserSplitterStretchFactor;
    const QString settingsFileName = "pcsmasettings";
    void loadSettings();
    void saveSettings();
};

#endif // SETTINGS_H
