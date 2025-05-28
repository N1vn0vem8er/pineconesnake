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

private:
    Ui::EventWidget *ui;
    EventManager::Event event;
    void refresh();
};

#endif // EVENTWIDGET_H
