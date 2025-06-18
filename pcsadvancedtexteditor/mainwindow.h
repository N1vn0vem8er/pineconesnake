#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>

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
    QLabel* pathLabel = nullptr;
    void openDir(const QString& path);
    void addTab(QWidget* widget, const QString& title);
    void saveFile(const QString &path, const QString &text);
    void openSaveWarningDialog(const QString path);
    void openFile(const QString &path);
    void loadHunspell();

private slots:
    void newFilePressed();
    void openFilePressed();
    void openDirPressed();
    void saveAs();
    void save();
    void open();
    void closeTab(const int index);
    void tabChanged(const int index);
    void copy();
    void selectAll();
    void cut();
    void paste();
    void undo();
    void redo();
    void showSearch();
    void find(const QString& text);
    void replace(const QString& find, const QString& replace);
    void deleteSelected();
    void deleteAll();
    void spellCheckSwitch(bool val);
};
#endif // MAINWINDOW_H
