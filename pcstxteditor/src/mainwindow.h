#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
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

private slots:
    void addTab(QWidget* widget, const QString& name);
    void openFileDialog();
    void openNewFile();
    void closeTab(const int index);
    void tabChanged(const int index);
    void saveAs();
    void save();
    void displayAboutDialog();
    void copy();
    void selectAll();
    void cut();
    void paste();
    void undo();
    void redo();
    void showSearch();
    void find(const QString& text);
    void replace(const QString& find, const QString& replace);
    void fontSizeChanged(int size);
    void increaseFontSize();
    void decreaseFontSize();
    void setFontSize();
    void setFont();
    void resetFontSize();
    void deleteText();
    void deleteAllText();

private:
    Ui::MainWindow *ui;
    QLabel* pathLabel {nullptr};
    int defaultFontSize {10};
    void openFile(const QString &path);
    void saveFile(const QString& path, const QString& text);
    void openSaveWarningDialog(const QString path);
};
#endif // MAINWINDOW_H
