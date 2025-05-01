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
    QThread* searchThread = nullptr;

private slots:
    void startMusicSearch();
    void searchResoultsReady(const QList<Track> tracks);

signals:
    void startSearch(const QString& path);
};
#endif // MAINWINDOW_H
