#include "settings.h"
#include <QDir>

Settings::Settings() {
    if(!QDir(databasePath).exists())
    {
        QDir().mkpath(databasePath);
    }
    if(!QDir(imagesPath).exists())
    {
        QDir().mkpath(imagesPath);
    }
}

QString Settings::databaseName = "database.db";
QString Settings::databasePath = QDir::homePath() + "/.local/share/pcscontenttracker";
QString Settings::imagesPath = QDir::homePath() + "/.local/share/pcscontenttracker/images";
