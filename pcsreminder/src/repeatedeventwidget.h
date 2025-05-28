#ifndef REPEATEDEVENTWIDGET_H
#define REPEATEDEVENTWIDGET_H


#include "eventmanager.h"

#include <QWidget>

namespace Ui {
class RepeatedEventWidget;
}

class RepeatedEventWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RepeatedEventWidget(const EventManager::RepeatedEvent &event, QWidget *parent = nullptr);
    ~RepeatedEventWidget();

private:
    Ui::RepeatedEventWidget *ui;
    EventManager::RepeatedEvent event;
    void refresh();

private slots:
    void editPressed();
    void deletePressed();

signals:
    void requestRefresh();
};

#endif // REPEATEDEVENTWIDGET_H
