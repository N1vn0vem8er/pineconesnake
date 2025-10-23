#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QMediaFormat>
#include <QFileDialog>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QMessageBox>
#include <QSettings>
#define VERSION "0.1.0"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"
#define PROJECTLINK "https://github.com/N1vn0vem8er/pineconesnake"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcsaudiorecorder");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    ui->actionCloseFileAfterRecording->setChecked(settings.value("closeFileAfterRecording", false).toBool());
    recorder = new QMediaRecorder(this);
    ui->fileFormatComboBox->addItem(tr("Default"));
    ui->codecsComboBox->addItem(tr("Default"));
    ui->inputsComboBox->addItem(tr("Default"));
    ui->qualityComboBox->addItem(tr("Default"));
    ui->encodeingModeComboBox->addItem(tr("Default"));
    openedFileLabel = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(openedFileLabel);
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
    for(const QString& i : qualityMap.keys())
    {
        ui->qualityComboBox->addItem(i);
    }
    for(const QString& i : encodingModeMap.keys())
    {
        ui->encodeingModeComboBox->addItem(i);
    }
    connect(ui->selectFileButton, &QPushButton::clicked, this, &MainWindow::selectFile);
    connect(ui->recordButton, &QPushButton::clicked, this, &MainWindow::record);
    connect(recorder, &QMediaRecorder::durationChanged, this, [&](qint64 val){ui->durationLabel->setText(QString("%1:%2:%3").arg(((val / 1000) / 60) % 24).arg(((val / 1000) / 60) % 60).arg((val / 1000) % 60));});
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::selectFile);
    connect(ui->actionClose, &QAction::triggered, this, [&]{selectedFile.clear(); openedFileLabel->clear();});
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    connect(recorder, &QMediaRecorder::recorderStateChanged, this, [&](QMediaRecorder::RecorderState state){
        if(state == QMediaRecorder::RecorderState::StoppedState)
        {
            ui->actionClose->setEnabled(true);
            if(ui->actionCloseFileAfterRecording->isChecked())
            {
                selectedFile.clear();
                openedFileLabel->clear();
            }
        }
        else
        {
            ui->actionClose->setEnabled(false);
        }
    });
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [&]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_PCS_Audio_Recorder, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->pauseButton, &QPushButton::clicked, this, [&]{if(recorder->recorderState() == QMediaRecorder::RecorderState::RecordingState) recorder->pause();
        else if(recorder->recorderState() == QMediaRecorder::RecorderState::PausedState) recorder->record();});
}

MainWindow::~MainWindow()
{
    QSettings settings("pcsaudiorecorder");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("closeFileAfterRecording", ui->actionCloseFileAfterRecording->isChecked());
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
        openedFileLabel->setText(selectedFile);
    }
}

void MainWindow::record()
{
    if(selectedFile.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Select file before recording"));
        return;
    }
    if(recorder->recorderState() == QMediaRecorder::StoppedState)
    {
        ui->recordButton->setText(tr("Stop"));
        session = new QMediaCaptureSession(recorder);
        audioInput = new QAudioInput(session);
        if(ui->inputsComboBox->currentIndex() != 0)
            audioInput->setDevice(devicesMap.value(ui->inputsComboBox->currentText()));
        session->setAudioInput(audioInput);
        session->setRecorder(recorder);
        QMediaFormat format;
        if(ui->fileFormatComboBox->currentIndex() != 0)
            format.setFileFormat(formatsMap.value(ui->fileFormatComboBox->currentText()));
        if(ui->codecsComboBox->currentIndex() != 0)
            format.setAudioCodec(codecsMap.value(ui->codecsComboBox->currentText()));
        recorder->setMediaFormat(format);
        if(ui->encodeingModeComboBox->currentIndex() != 0)
            recorder->setEncodingMode(encodingModeMap.value(ui->encodeingModeComboBox->currentText()));
        if(ui->qualityComboBox->currentIndex() != 0)
            recorder->setQuality(qualityMap.value(ui->qualityComboBox->currentText()));
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

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About PCS Audio Recorder"), tr(R"(
    <html>
        <body>
            <h3>PCS Audio Recorder</h3>
            <p>PCS Audio Recorder is a simple audio recorder. It is a part of the <a href="%1">Pinecone Snake</a> project.</p>
            <p>Version: %2</p>
            <p>License: <a href="%3">GPL 3</a></p>
        </body>
    </html>
)").arg(PROJECTLINK, VERSION, LICENSELINK));
}
