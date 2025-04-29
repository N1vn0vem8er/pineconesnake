#include "settings.h"

#include <QSettings>

Settings::Settings() {}

void Settings::loadSettings()
{
    QString dbDefaultPath = QDir().homePath() + "/.local/share/pineconesnake/pcsmessagesarchiver";
    if(!QDir(dbDefaultPath).exists())
    {
        QDir().mkpath(dbDefaultPath);
    }
    QSettings settings(settingsFileName);
    databasePath = settings.value("database.path", dbDefaultPath).toString();
    databaseName = settings.value("database.name", "database.db").toString();
    browserSplitterStretchFactor = settings.value("browserSplitterStretchFactor", "0").toInt();
    mainSplitterStretchFactor = settings.value("mainSplitterStretchFactor", "2").toInt();
    theme = settings.value("theme", "").toString();
}

void Settings::saveSettings()
{
    QSettings settings(settingsFileName);
    settings.setValue("database.path", databasePath);
    settings.setValue("database.name", databaseName);
    settings.setValue("theme", theme);
    settings.setValue("browserSplitterStretchFactor", browserSplitterStretchFactor);
    settings.setValue("mainSplitterStretchFactor", mainSplitterStretchFactor);
}
QString Settings::databasePath;
QString Settings::databaseName;
QString Settings::theme = "";
int Settings::browserSplitterStretchFactor = 0;
int Settings::mainSplitterStretchFactor = 2;
