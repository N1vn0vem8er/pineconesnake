#ifndef CREATEEVENTDIALOG_H
#define CREATEEVENTDIALOG_H

#include "eventmanager.h"
#include <QDialog>

namespace Ui {
class CreateEventDialog;
}

class CreateEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateEventDialog(QWidget *parent = nullptr);
    ~CreateEventDialog();

private:
    Ui::CreateEventDialog *ui;

private slots:
    void okPressed();
    void optionChanged(int index);

signals:
    void addEvent(const EventManager::Event& event);
    void addRepeating(const EventManager::RepeatedEvent& event);
};

#endif // CREATEEVENTDIALOG_H
