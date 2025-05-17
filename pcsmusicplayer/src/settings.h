#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    Settings();
    static QString databasePath;
    static QString databaseName;
    static QStringList searchDirs;
    static float volume;
    const QString settingsFileName = "pcsmusicplayer";
    void loadSettings();
    void saveSettings();
};

#endif // SETTINGS_H
