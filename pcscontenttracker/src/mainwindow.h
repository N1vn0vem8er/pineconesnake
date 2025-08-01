#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "structs.h"
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
    void refresh();
    enum SelectedModes{
        All,
        Finished,
        Planned,
        Current,
        Search
    };
    int selected = SelectedModes::All;

private slots:
    void addNewItem();
    void editItem(Item &item);
    void deleteItem(const Item& item);
    void displayFinished();
    void displayPlanned();
    void displayCurrent();
    void displayAll();
    void search();
    void openImagesDir();
    void openAbout();
    void editSelected();
    void deleteSelected();
};
#endif // MAINWINDOW_H
