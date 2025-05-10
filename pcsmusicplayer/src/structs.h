#ifndef STRUCTS_H
#define STRUCTS_H

#include "qobject.h"

struct Track
{
    Track(){};
    Track(int id, QString title, QString path, QString artist, QString album, int number, int length, int played, bool favorite) :
        id(id), title(title), path(path), artist(artist), album(album), number(number), length(length), played(played), favorite(favorite){}
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

struct Playlist
{
    Playlist(){};
    Playlist(int id, QString name, QList<Track> tracks) : id(id), name(name), tracks(tracks){};
    int id;
    QString name;
    QList<Track> tracks;
};

#endif // STRUCTS_H
