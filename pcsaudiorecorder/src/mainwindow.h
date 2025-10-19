#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaFormat>
#include <QMediaRecorder>
#include <qmediacapturesession.h>

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
    QMediaRecorder* recorder {nullptr};
    QMediaCaptureSession* session {nullptr};
    QAudioInput* audioInput {nullptr};

private:
    QString getFileFormatName(QMediaFormat::FileFormat format) const;
    QString getCodecName(QMediaFormat::AudioCodec codec) const;

private slots:
    void selectFile();
    void record();
};
#endif // MAINWINDOW_H
