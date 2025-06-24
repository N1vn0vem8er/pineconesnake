#ifndef SETTINGS_H
#define SETTINGS_H

#include <qobject.h>
class Settings
{
public:
    Settings();
    static QString defaultLanguage;
    static bool spellCheckEnabled;
};

#endif // SETTINGS_H
