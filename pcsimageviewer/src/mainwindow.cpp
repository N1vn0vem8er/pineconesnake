#include "mainwindow.h"
#include "qevent.h"
#include "qlabel.h"
#include "src/dialogs/propertiesdialog.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>


#define VERSION "1.0.0"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openImage);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::closeImage);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_Application, &QAction::triggered, this, &MainWindow::displayAboutApplication);
    connect(ui->actionRotateLeft, &QAction::triggered, this, &MainWindow::rotateImageLeft);
    connect(ui->actionRotate_Right, &QAction::triggered, this, &MainWindow::rotateImageRight);
    connect(ui->rotateRightButton, &QPushButton::clicked, this, &MainWindow::rotateImageRight);
    connect(ui->rotateLeftButton, &QPushButton::clicked, this, &MainWindow::rotateImageLeft);
    connect(ui->actionProperties, &QAction::triggered, this, &MainWindow::displayFileProperties);
    connect(ui->horizontalSlider, &QSlider::actionTriggered, this, &MainWindow::zoom);
    connect(ui->imageView, &ImageView::scaleChanged, this, &MainWindow::changeScaleSlider);
    connect(ui->actionNext, &QAction::triggered, this, &MainWindow::openImageRight);
    connect(ui->actionClearRecent, &QAction::triggered, this, &MainWindow::clearRecent);
    connect(ui->actionPrevious, &QAction::triggered, this, &MainWindow::openImegeLeft);

    if(qApp->arguments().length() == 2)
    {
        open(qApp->arguments()[1]);
    }

    setVisibility(false);
    imagePathLabel = new QLabel(ui->statusbar);
    ui->statusbar->addWidget(imagePathLabel);
    loadRecent();
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::openImage(){
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QDir::homePath(), tr("Images (*.png *.jpg)"));
    if(!fileName.isEmpty() && fileName != ""){
        open(fileName);
    }
}

void MainWindow::closeImage(){
    if(!openedImage.isEmpty()){
        ui->imageView->setImage(QImage());
        openedImage.clear();
        setVisibility(false);
        imagePathLabel->setText("");
    }
}

void MainWindow::loadRecentFiles(){

}

void MainWindow::displayAboutApplication(){
    QMessageBox::about(this, tr("About Application"), tr("<html><body><h2>Image Viewer</h2><p>PCS Image Viewer is a simple image viewer. It is a part of the Pinecone Snake project.</p><p>Version: %1</p><p>License: <a href=\"%2\">GPL 3</a></p></body></html>")
                                                          .arg(VERSION)
                                                          .arg(LICENSELINK));
}

void MainWindow::rotateImageRight(){
    ui->imageView->rotateRight();
}

void MainWindow::rotateImageLeft()
{
    ui->imageView->rotateLeft();
}

void MainWindow::displayFileProperties(){
    if(!openedImage.isEmpty()){
        PropertiesDialog dialog(openedImage, this);
        dialog.exec();
    }
}

void MainWindow::zoom(){
    if(!openedImage.isEmpty()){
        int newPos = ui->horizontalSlider->value();
        if(newPos > sliderPosition)
        {
            ui->imageView->setScale(1.1);
        }
        else
        {
            ui->imageView->setScale(0.9);
        }
        sliderPosition = newPos;
    }
}

void MainWindow::saveToRecent()
{
    QFile file(getRecentPath());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(file.isOpen())
    {
        QStringList paths;
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            paths << stream.readLine();
        }
        file.close();
        if(!paths.contains(openedImage))
        {
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            if(file.isOpen())
            {
                QTextStream stream(&file);
                if(paths.length() > 10) paths.removeAt(0);
                paths.append(openedImage);
                for(const auto& i : paths)
                {
                    stream << i + "\n";
                }
                file.close();
            }
        }
    }
    ui->menuRecent->clear();
    loadRecent();
}

void MainWindow::loadRecent()
{
    QFile file(getRecentPath());
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QTextStream stream(&file);
        if(!stream.atEnd())
            ui->menuRecent->clear();
        while(!stream.atEnd())
        {
            const QString line = stream.readLine();
            QAction* action = new QAction(ui->menuRecent);
            action->setText(line);
            connect(action, &QAction::triggered, this, [=]{open(line);});
            ui->menuRecent->addAction(action);
        }
        file.close();
    }
}

void MainWindow::open(const QString &path)
{
    QImage image;
    if(image.load(path)){
        ui->imageView->setImage(image);
        openedImage = path;
        setVisibility(true);
        imagePathLabel->setText(path);
        saveToRecent();
    }
}

void MainWindow::openImegeLeft()
{
    QFileInfoList images = getImagesInDirectory();
    int index = images.indexOf(QFileInfo(openedImage));
    if(index < 0)
        return;
    if(index == 0)
    {
        closeImage();
        open(images[images.length()-1].absoluteFilePath());
    }
    else
    {
        closeImage();
        open(images[index-1].absoluteFilePath());
    }
}

void MainWindow::openImageRight()
{
    QFileInfoList images = getImagesInDirectory();
    int index = images.indexOf(QFileInfo(openedImage));
    if(index < 0)
        return;
    if(index >= images.size() || index + 1 >= images.size())
    {
        closeImage();
        open(images[0].absoluteFilePath());
    }
    else
    {
        closeImage();
        open(images[index+1].absoluteFilePath());
    }
}

QFileInfoList MainWindow::getImagesInDirectory()
{
    QString dirPath = QFileInfo(openedImage).absolutePath();
    QDir directory(dirPath);
    QFileInfoList images = directory.entryInfoList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.jpeg" << "*.JPEG", QDir::Files);
    return images;
}

void MainWindow::clearRecent()
{
    QFile file(getRecentPath());
    file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
    if(file.isOpen())
    {
        file.write("");
        file.close();
    }
    ui->menuRecent->clear();
    loadRecent();
}

void MainWindow::setVisibility(bool val)
{
    ui->actionClose->setVisible(val);
    ui->actionProperties->setVisible(val);
    ui->menuView->menuAction()->setVisible(val);
    ui->horizontalSlider->setVisible(val);
    ui->rotateLeftButton->setVisible(val);
    ui->rotateRightButton->setVisible(val);
    ui->actionNext->setVisible(val);
    ui->actionPrevious->setVisible(val);
}

QString MainWindow::getRecentPath() const
{
    QString path = QDir::homePath() + "/.local/share/pineconesnake/pcsimageviewer";
    if(!QDir(path).exists())
        QDir(path).mkpath(path);
    path += "/recent.txt";
    if(!QFileInfo(path).exists())
    {
        QFile file(path);
        file.open(QIODevice::WriteOnly);
        if(file.isOpen())
        {
            file.write(0);
            file.close();
        }
    }
    return path;
}

void MainWindow::changeScaleSlider(double factor)
{
    ui->horizontalSlider->setValue(factor == 1.1 ? ui->horizontalSlider->value()+1 : ui->horizontalSlider->value()-1);
}
