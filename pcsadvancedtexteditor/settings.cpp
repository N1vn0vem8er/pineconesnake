#include "settings.h"
#include <qdir.h>

Settings::Settings() {}

QString Settings::defaultLanguage = "en_US";
bool Settings::spellCheckEnabled {true};
QString Settings::storagePath = QDir::homePath() + "/.local/share/pineconesnake/pcsadvancedtexteditor";
QString Settings::recentFilesStoragePath = storagePath + "/recentFiles.txt";
QString Settings::recentDirsStoragePath = storagePath + "/recentDirs.txt";
int Settings::defaultFontSize = 10;
