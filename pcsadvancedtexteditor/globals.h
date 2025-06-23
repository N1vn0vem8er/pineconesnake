#ifndef GLOBALS_H
#define GLOBALS_H

#include "structs.h"
#include <qobject.h>
class Globals
{
public:
    Globals();
    static QStringList hunspellLanguages;
    static QList<MimeApplication> apps;
};

#endif // GLOBALS_H
