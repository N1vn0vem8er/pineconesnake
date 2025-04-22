#ifndef STRUCTS_H
#define STRUCTS_H
#include <QString>

struct Item
{
    Item(){};
    Item(int id, QString title, QString description, int status, QString image, QString link, int chapter, int chapters) : id(id), title(title), description(description), status(status), image(image), link(link), chapter(chapter), chapters(chapters){};
    int id;
    QString title;
    QString description;
    int status;
    QString image;
    QString link;
    int chapter;
    int chapters;
};

struct Status
{
    Status(int id, QString name) : id(id), name(name){};
    int id;
    QString name;
};

#endif // STRUCTS_H
