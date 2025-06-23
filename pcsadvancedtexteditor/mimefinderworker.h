#ifndef MIMEFINDERWORKER_H
#define MIMEFINDERWORKER_H

#include "structs.h"
#include <QObject>

class MimeFinderWorker : public QObject
{
    Q_OBJECT
public:
    explicit MimeFinderWorker(QObject *parent = nullptr);

public slots:
    void start();

private:
    QList<MimeApplication> find(const QString& path);

signals:
};

#endif // MIMEFINDERWORKER_H
