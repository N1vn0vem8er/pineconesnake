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

private:
    Ui::MainWindow *ui;

private slots:
    void newFile();
    void openFile();
    void openDir();
    void save();
    void saveAs();
    void closeTab(int index);
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void deleteText();
    void deleteAllText();
    void selectAll();
};
#endif // MAINWINDOW_H
