#ifndef STRUCTS_H
#define STRUCTS_H
#include <QString>

struct Note{
    Note(){}
    Note(int id, QString title, QString content, QString created, QString modified) : id(id), title(title), content(content), created(created), modified(modified){}
    int id;
    QString title;
    QString content;
    QString created;
    QString modified;
};

#endif // STRUCTS_H
