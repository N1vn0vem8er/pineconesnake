#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "eventmanager.h"
#include "qboxlayout.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVBoxLayout* layout = nullptr;
    QList<QWidget*> widgets;
    QList<EventManager::RepeatedEvent> repeatedEvents;
    QList<EventManager::Event> events;

private slots:
    void refresh();
    void addReminderPressed();
    void addReminder(const EventManager::Event& event);
    void addRepeating(const EventManager::RepeatedEvent& event);

signals:
    void requestRefresh();

};
#endif // MAINWINDOW_H
