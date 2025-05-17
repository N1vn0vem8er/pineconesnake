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
    searchDirs.clear();
    for(int i =0 ;i<settings.beginReadArray("dirs");i++)
    {
        settings.setArrayIndex(i);
        QString dir = settings.value("dir", "").toString();
        if(!dir.isEmpty())searchDirs.append(dir);
    }
    settings.endArray();
    if(searchDirs.isEmpty())
        searchDirs.append(QDir::homePath() + "/Music");
}

void Settings::saveSettings()
{
    QSettings settings(settingsFileName);
    settings.setValue("volume", volume);
    settings.beginWriteArray("dirs");
    for(int i =0 ;i<searchDirs.size();i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("dir", searchDirs[i]);
    }
    settings.endArray();
}

QString Settings::databasePath = QDir::homePath() + "/.local/share/pineconesnake/pcsmusicplayer";
QString Settings::databaseName = "database.db";
float Settings::volume = 0.0;
QStringList Settings::searchDirs;
