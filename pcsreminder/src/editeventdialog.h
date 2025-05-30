#ifndef EDITEVENTDIALOG_H
#define EDITEVENTDIALOG_H

#include "eventmanager.h"
#include <QDialog>

namespace Ui {
class EditEventDialog;
}

class EditEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditEventDialog(const EventManager::Event& event, QWidget *parent = nullptr);
    explicit EditEventDialog(const EventManager::RepeatedEvent& event, QWidget *parent = nullptr);
    ~EditEventDialog();

private:
    Ui::EditEventDialog *ui;
    EventManager::Event event;
    EventManager::RepeatedEvent repeated;
    int type;
    void init();

private slots:
    void submit();
    void optionChanged(int index);

signals:
    void editEvent(const EventManager::Event& event);
    void editRepeating(const EventManager::RepeatedEvent& event);
};

#endif // EDITEVENTDIALOG_H
