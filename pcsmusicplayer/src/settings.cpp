#include "settings.h"
#include "qdir.h"

Settings::Settings()
{
    if(!QDir(databasePath).exists())
    {
        QDir().mkpath(databasePath);
    }
}

QString Settings::databasePath = QDir::homePath() + "/.local/share/pineconesnake/pcsmusicplayer";
QString Settings::databaseName = "database.db";

