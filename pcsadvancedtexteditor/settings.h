#ifndef SETTINGS_H
#define SETTINGS_H

#include <qobject.h>
class Settings
{
public:
    Settings();
    static QString defaultLanguage;
    static bool spellCheckEnabled;
    static QString recentFilesStoragePath;
    static QString recentDirsStoragePath;
    static QString storagePath;
};

#endif // SETTINGS_H
