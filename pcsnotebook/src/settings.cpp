#include "settings.h"
#include <QDir>

Settings::Settings()
{
    if(!QDir(databasePath).exists())
    {
        QDir().mkpath(databasePath);
    }
}

QString Settings::databasePath = QDir::homePath() + "/.local/share/pcsnotebook";
QString Settings::databaseName = "database.db";
