#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QMediaFormat>
#include <QFileDialog>
#include <QMediaCaptureSession>
#include <QAudioInput>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    recorder = new QMediaRecorder(this);
    ui->fileFormatComboBox->addItem(tr("Default"));
    ui->codecsComboBox->addItem(tr("Default"));
    ui->inputsComboBox->addItem(tr("Default"));
    for(const QAudioDevice& i : QMediaDevices::audioInputs())
    {
        devicesMap[i.description()] = i;
        ui->inputsComboBox->addItem(i.description());
    }
    for(const QMediaFormat::FileFormat& i : QMediaFormat().supportedFileFormats(QMediaFormat::ConversionMode()))
    {
        const QString name = getFileFormatName(i);
        formatsMap[name] = i;
        ui->fileFormatComboBox->addItem(name);
    }
    for(const QMediaFormat::AudioCodec& i : QMediaFormat().supportedAudioCodecs(QMediaFormat::ConversionMode()))
    {
        const QString name = getCodecName(i);
        codecsMap[name] = i;
        ui->codecsComboBox->addItem(name);
    }
    connect(ui->selectFileButton, &QPushButton::clicked, this, &MainWindow::selectFile);
    connect(ui->recordButton, &QPushButton::clicked, this, &MainWindow::record);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getFileFormatName(QMediaFormat::FileFormat format) const
{
    switch(format)
    {
    case QMediaFormat::UnspecifiedFormat:
        return "UnspecifiedFormat";
    case QMediaFormat::WMV:
        return "WMV";
    case QMediaFormat::AVI:
        return "AVI";
    case QMediaFormat::Matroska:
        return "Matroska";
    case QMediaFormat::MPEG4:
        return "MPEG4";
    case QMediaFormat::Ogg:
        return "Ogg";
    case QMediaFormat::QuickTime:
        return "QuickTime";
    case QMediaFormat::WebM:
        return "WebM";
    case QMediaFormat::Mpeg4Audio:
        return "Mpeg4Audio";
    case QMediaFormat::AAC:
        return "AAC";
    case QMediaFormat::WMA:
        return "WMA";
    case QMediaFormat::MP3:
        return "MP3";
    case QMediaFormat::FLAC:
        return "FLAC";
    case QMediaFormat::Wave:
        return "Wave";
    }
    return "";
}

QString MainWindow::getCodecName(QMediaFormat::AudioCodec codec) const
{
    switch(codec)
    {
    case QMediaFormat::AudioCodec::Unspecified:
        return "Unspecified";
    case QMediaFormat::AudioCodec::MP3:
        return "MP3";
    case QMediaFormat::AudioCodec::AAC:
        return "AAC";
    case QMediaFormat::AudioCodec::AC3:
        return "AC3";
    case QMediaFormat::AudioCodec::EAC3:
        return "EAC3";
    case QMediaFormat::AudioCodec::FLAC:
        return "FLAC";
    case QMediaFormat::AudioCodec::DolbyTrueHD:
        return "DolbyTrueHD";
    case QMediaFormat::AudioCodec::Opus:
        return "Opus";
    case QMediaFormat::AudioCodec::Vorbis:
        return "Vorbis";
    case QMediaFormat::AudioCodec::Wave:
        return "Wave";
    case QMediaFormat::AudioCodec::WMA:
        return "WMA";
    case QMediaFormat::AudioCodec::ALAC:
        return "ALAC";
        break;
    }
    return "";
}

void MainWindow::selectFile()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Select File"), QDir::homePath());
    if(!path.isEmpty())
    {
        selectedFile = path;
    }
}

void MainWindow::record()
{
    if(selectedFile.isEmpty())
        return;
    if(recorder->recorderState() == QMediaRecorder::StoppedState)
    {
        ui->recordButton->setText(tr("Stop"));
        session = new QMediaCaptureSession(recorder);
        audioInput = new QAudioInput(session);
        if(ui->inputsComboBox->currentIndex() != 0)
            audioInput->setDevice(devicesMap[ui->inputsComboBox->currentText()]);
        session->setAudioInput(audioInput);
        session->setRecorder(recorder);
        QMediaFormat format;
        if(ui->fileFormatComboBox->currentIndex() != 0)
            format.setFileFormat(formatsMap[ui->fileFormatComboBox->currentText()]);
        if(ui->codecsComboBox->currentIndex() != 0)
            format.setAudioCodec(codecsMap[ui->codecsComboBox->currentText()]);
        recorder->setMediaFormat(format);
        recorder->setOutputLocation(QUrl::fromLocalFile(selectedFile));
        recorder->record();
    }
    else if(recorder->recorderState() == QMediaRecorder::RecordingState || recorder->recorderState() == QMediaRecorder::PausedState)
    {
        ui->recordButton->setText(tr("Record"));
        if(session)
            session->deleteLater();
        recorder->stop();
    }
}
