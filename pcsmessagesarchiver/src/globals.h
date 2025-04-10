#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
namespace Globals{
    struct Message{
        Message(){};
        Message(int id, QString title, QString contents, QString from, QString to, QString created, QString modified) : id(id), title(title), contents(contents), from(from), to(to), created(created), modified(modified){};
        int id;
        QString title;
        QString contents;
        QString from;
        QString to;
        QString created;
        QString modified;
    };
    struct Contact{
        Contact(){};
        Contact(int id, QString name) : id(id), name(name){};
        int id;
        QString name;
    };
}

#endif // GLOBALS_H
