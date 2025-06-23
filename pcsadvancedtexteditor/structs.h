#ifndef STRUCTS_H
#define STRUCTS_H

#include <QObject>


struct MimeApplication{
    int id;
    QString name;
    QList<QString> mimeTypes;
};


#endif // STRUCTS_H
