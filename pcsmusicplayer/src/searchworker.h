#ifndef SEARCHWORKER_H
#define SEARCHWORKER_H

#include "structs.h"
#include <QObject>

class SearchWorker : public QObject
{
    Q_OBJECT
public:
    explicit SearchWorker(QObject *parent = nullptr);

public slots:
    void doWork(const QString& path);

signals:
    void resultReady(const QList<Track>& tracks);
};

#endif // SEARCHWORKER_H
