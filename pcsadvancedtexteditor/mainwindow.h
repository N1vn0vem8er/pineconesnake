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
    void open(const QString& path);
    void openDir(const QString& path);
    void addTab(QWidget* widget, const QString& title);
    void saveFile(const QString &path, const QString &text);

    void openSaveWarningDialog(const QString path);
private slots:
    void newFilePressed();
    void openFilePressed();
    void openDirPressed();
    void saveAs();
    void save();
    void closeTab(const int index);
};
#endif // MAINWINDOW_H
