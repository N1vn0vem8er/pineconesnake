#ifndef STRUCTS_H
#define STRUCTS_H

#include <QObject>


struct MimeApplication{
    QString name;
    QString icon;
    QString exec;
    QList<QString> mimeTypes;
};

struct GitFileStatus{
    GitFileStatus() {};
    GitFileStatus(QString name, QString path, QString status, QString addedLines, QString removedLines) : name(name), path(path), status(status), addedLines(addedLines), removedLines(removedLines) {};
    QString name;
    QString path;
    QString status;
    QString addedLines;
    QString removedLines;
};


#endif // STRUCTS_H
