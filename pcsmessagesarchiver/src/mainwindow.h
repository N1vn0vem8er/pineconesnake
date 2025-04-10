#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
public slots:
    void addTab(QWidget* tab, const QString& title);
    void removeTab(int index);
private:
    Ui::MainWindow *ui;
    void addNewHomeWidget();
private slots:
    void openFromRecent(int id);
    void openAboutWidget();
};
#endif // MAINWINDOW_H
