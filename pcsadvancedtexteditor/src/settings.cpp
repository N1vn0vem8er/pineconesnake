#include "settings.h"
#include <QSettings>
#include <qdir.h>

Settings::Settings() {}

void Settings::load()
{
    QSettings settings(settingsPath);
    defaultLanguage = settings.value("defaultLanguage", "en_US").toString();
    spellCheckEnabled = settings.value("spellCheckEnabled", true).toBool();
    defaultFontSize = settings.value("defaultFontSize", 10).toInt();
    recentFilesLimit = settings.value("recentFilesLimit", 10).toInt();
    recentDirsLimit = settings.value("recentDirsLimit", 10).toInt();
    defaultFileTreeDir = settings.value("defaultFileTreeDir", QDir::homePath()).toString();
    hunspellDirs = settings.value("hunspellDirs", {"/usr/share/hunspell"}).toStringList();
}

void Settings::save()
{
    QSettings settings(settingsPath);
    settings.setValue("defaultLanguage", defaultLanguage);
    settings.setValue("spellCheckEnabled", spellCheckEnabled);
    settings.setValue("defaultFontSize", defaultFontSize);
    settings.setValue("recentFilesLimit", recentFilesLimit);
    settings.setValue("recentDirsLimit", recentDirsLimit);
    settings.setValue("defaultFileTreeDir", defaultFileTreeDir);
    settings.setValue("hunspellDirs", hunspellDirs);
}

QString Settings::defaultLanguage = "en_US";
bool Settings::spellCheckEnabled {true};
QString Settings::storagePath = QDir::homePath() + "/.local/share/pineconesnake/pcsadvancedtexteditor";
QString Settings::recentFilesStoragePath = storagePath + "/recentFiles.txt";
QString Settings::recentDirsStoragePath = storagePath + "/recentDirs.txt";
int Settings::defaultFontSize = 10;
QStringList Settings::hunspellDirs {"/usr/share/hunspell"};
int Settings::recentFilesLimit = 10;
int Settings::recentDirsLimit = 10;
QString Settings::defaultFileTreeDir = QDir::homePath();
