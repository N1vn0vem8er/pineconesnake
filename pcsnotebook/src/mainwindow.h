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
    void addTab(const QString& title, QWidget* widget);

private slots:
    void addNewNote();
    void showAllNotes();
    void openWriter(const Note& note);
    void refreshNotes();
    void save();
    void copy();
    void cut();
    void paste();
    void selectAll();
    void saveAs();
    void undo();
    void redo();
    void deleteRequested(const Note &note);
    void displayAbout();
};
#endif // MAINWINDOW_H
