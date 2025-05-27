#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QMap>
#include <QObject>
#include <QSystemTrayIcon>

class EventManager : public QObject
{
    Q_OBJECT
public:
    explicit EventManager(QObject *parent = nullptr);
    ~EventManager();
    struct Event{
        Event(){};
        Event(int id, QString title, QString content, QString date, bool enabled) : id(id), title(title), content(content),  date(date), enabled(enabled){}
        int id;
        QString title;
        QString content;
        QString date;
        bool enabled;
    };
    struct RepeatedEvent{
        RepeatedEvent(){}
        RepeatedEvent(int id, QString title, QString content, int everySeconds, bool enabled) : id(id), title(title), content(content), everySeconds(everySeconds), enabled(enabled){}
        int id;
        QString title;
        QString content;
        int everySeconds;
        bool enabled;
    };

private:
    QSystemTrayIcon* icon = nullptr;
    QMenu* menu = nullptr;
    QTimer* timer = nullptr;
    QList<QPair<RepeatedEvent, int>> repeatedEvents;
    QList<Event> events;

private slots:
    void close();
    void openSettings();
    void checkEvents();
    void refresh();

signals:
};

#endif // EVENTMANAGER_H
