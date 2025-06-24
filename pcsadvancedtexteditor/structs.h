#ifndef STRUCTS_H
#define STRUCTS_H

#include <QObject>


struct MimeApplication{
    QString name;
    QString icon;
    QList<QString> mimeTypes;
};


#endif // STRUCTS_H
