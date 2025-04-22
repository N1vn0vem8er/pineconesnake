#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>


class Settings
{
public:
    Settings();
    static QString databasePath;
    static QString databaseName;
    static QString imagesPath;
};

#endif // SETTINGS_H
