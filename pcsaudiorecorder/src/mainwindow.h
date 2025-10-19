#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaFormat>
#include <QMediaRecorder>
#include <qaudiodevice.h>
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
    QMap<QString, QMediaFormat::AudioCodec> codecsMap;
    QMap<QString, QMediaFormat::FileFormat> formatsMap;
    QMap<QString, QAudioDevice> devicesMap;
    const QMap<QString, QMediaRecorder::Quality> qualityMap {
        {"VeryLowQuality", QMediaRecorder::Quality::VeryLowQuality},
        {"LowQuality", QMediaRecorder::Quality::LowQuality},{"NormalQuality", QMediaRecorder::Quality::NormalQuality},
        {"HighQuality", QMediaRecorder::Quality::HighQuality},{"VeryHighQuality", QMediaRecorder::Quality::VeryHighQuality}
    };
    const QMap<QString, QMediaRecorder::EncodingMode> encodingModeMap{
        {"AverageBitRateEncoding", QMediaRecorder::EncodingMode::AverageBitRateEncoding},{"ConstantBitRateEncoding", QMediaRecorder::EncodingMode::ConstantBitRateEncoding},
        {"ConstantQualityEncoding", QMediaRecorder::EncodingMode::ConstantQualityEncoding},{"TwoPassEncoding", QMediaRecorder::EncodingMode::TwoPassEncoding}
    };

private:
    QString getFileFormatName(QMediaFormat::FileFormat format) const;
    QString getCodecName(QMediaFormat::AudioCodec codec) const;

private slots:
    void selectFile();
    void record();
};
#endif // MAINWINDOW_H
