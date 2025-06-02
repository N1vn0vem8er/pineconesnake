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
        Event(int id, QString title, QString content, QString date, bool enabled, int type) : id(id), title(title), content(content),  date(date), enabled(enabled), type(type){}
        int id;
        QString title;
        QString content;
        QString date;
        bool enabled;
        int type;
    };
    struct RepeatedEvent{
        RepeatedEvent(){}
        RepeatedEvent(int id, QString title, QString content, int everySeconds, bool enabled, int type) : id(id), title(title), content(content), everySeconds(everySeconds), enabled(enabled), type(type){}
        int id;
        QString title;
        QString content;
        int everySeconds;
        bool enabled;
        int type;
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
