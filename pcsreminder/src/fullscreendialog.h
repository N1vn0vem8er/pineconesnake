#ifndef FULLSCREENDIALOG_H
#define FULLSCREENDIALOG_H

#include "eventmanager.h"

#include <QDialog>

namespace Ui {
class FullScreenDialog;
}

class FullScreenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FullScreenDialog(const EventManager::Event& event, QWidget *parent = nullptr);
    explicit FullScreenDialog(const EventManager::RepeatedEvent& event, QWidget *parent = nullptr);
    ~FullScreenDialog();

private:
    Ui::FullScreenDialog *ui;
    EventManager::Event event;
    EventManager::RepeatedEvent repeated;
    int type;
    void init();

private slots:
    void disable();

signals:
    void requestRefresh();
};

#endif // FULLSCREENDIALOG_H
