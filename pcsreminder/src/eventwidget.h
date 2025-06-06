#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include "eventmanager.h"
#include <QWidget>

namespace Ui {
class EventWidget;
}

class EventWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventWidget(const EventManager::Event& event ,QWidget *parent = nullptr);
    ~EventWidget();

public slots:
    void enable();
    void disable();

private:
    Ui::EventWidget *ui;
    EventManager::Event event;
    void refresh();

private slots:
    void editPressed();
    void deletePressed();
    void editReminder(const EventManager::Event &event);

signals:
    void requestRefresh();
};

#endif // EVENTWIDGET_H
