#ifndef STRUCTS_H
#define STRUCTS_H

#include "qobject.h"

struct Track
{
    Track(){};
    int id;
    QString title;
    QString path;
    QString artist;
    QString album;
    int number;
    int length;
    int played;
    bool favorite;
};

#endif // STRUCTS_H
