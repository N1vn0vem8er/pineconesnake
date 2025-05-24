#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    enum ReminderOptions{
        Notification,
        Window,
        Fullscreen
    };

private:
    Ui::MainWindow *ui;
    QVBoxLayout* layout = nullptr;
    QList<QWidget*> widgets;

private slots:
    void refresh();
    void addReminderPressed();
    void addRepeatingPressed();

};
#endif // MAINWINDOW_H
