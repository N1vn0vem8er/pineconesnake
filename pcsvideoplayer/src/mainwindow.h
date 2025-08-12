#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>

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
    QMediaPlayer* player {nullptr};
    QAudioOutput* audioOutput {nullptr};

private slots:
    void openFile();
    void play();
    void pause();
    void seekForward();
    void seekBackward();
    void next();
    void previous();
};
#endif // MAINWINDOW_H
