#include "settings.h"
#include "qdir.h"

#include <QSettings>

Settings::Settings()
{
    if(!QDir(databasePath).exists())
    {
        QDir().mkpath(databasePath);
    }
}

void Settings::loadSettings()
{
    QSettings settings(settingsFileName);
    volume = settings.value("volume", 0.0).toFloat();
}

void Settings::saveSettings()
{
    QSettings settings(settingsFileName);
    settings.setValue("volume", volume);
}

QString Settings::databasePath = QDir::homePath() + "/.local/share/pineconesnake/pcsmusicplayer";
QString Settings::databaseName = "database.db";
float Settings::volume = 0.0;

