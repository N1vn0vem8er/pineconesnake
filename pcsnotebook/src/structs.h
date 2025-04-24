#ifndef STRUCTS_H
#define STRUCTS_H
#include <QString>

struct Note{
    Note(){}
    int id;
    QString title;
    QString content;
    QString created;
    QString modified;
};

#endif // STRUCTS_H
