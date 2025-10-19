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
    QStringList texts;
    QString openedFile;

private:
    void refresh();

private slots:
    void open();
    void save();
    void saveAs();
    void selectRandom();
    void add();
    void remove();
    void edited(const QString& oldText, const QString& newText);
    void removed(const QString& text);
};
#endif // MAINWINDOW_H
