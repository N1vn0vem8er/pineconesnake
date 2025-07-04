#ifndef SETTINGS_H
#define SETTINGS_H

#include <qdir.h>
#include <qobject.h>
class Settings
{
public:
    Settings();
    void load();
    void save();
    const QString settingsPath {"pcsadvancedtexteditor"};
    static QString defaultLanguage;
    static bool spellCheckEnabled;
    static QString recentFilesStoragePath;
    static QString recentDirsStoragePath;
    static QString storagePath;
    static int defaultFontSize;
    static QStringList hunspellDirs;
    static int recentFilesLimit;
    static int recentDirsLimit;
    static QString defaultFileTreeDir;
};

#endif // SETTINGS_H
