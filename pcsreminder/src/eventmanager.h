#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>

class EventManager : public QObject
{
    Q_OBJECT
public:
    explicit EventManager(QObject *parent = nullptr);
    ~EventManager();
    struct Event{
        int id;
        QString title;
        QString content;
        bool repeat = false;
        QDate date();
    };
    struct RepeatedEvent{
        int id;
        QString title;
        QString content;
        int everySeconds;
    };

private:
    QSystemTrayIcon* icon = nullptr;
    QMenu* menu = nullptr;

private slots:
    void close();
    void openSettings();

signals:
};

#endif // EVENTMANAGER_H
