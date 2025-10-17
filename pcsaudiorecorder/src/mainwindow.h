#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaFormat>

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
    QString selectedFile;

private:
    QString getFileFormatName(QMediaFormat::FileFormat format) const;
    QString getCodecName(QMediaFormat::AudioCodec codec) const;

private slots:
    void selectFile();
};
#endif // MAINWINDOW_H
