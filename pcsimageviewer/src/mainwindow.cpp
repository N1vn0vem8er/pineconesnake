#include "mainwindow.h"
#include "qevent.h"
#include "qlabel.h"
#include "qscrollarea.h"
#include "src/dialogs/propertiesdialog.h"
#include "src/imageview.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>


#define VERSION "1.0.2"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"
#define PROJECTLINK "https://github.com/N1vn0vem8er/pineconesnake"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openImage);
    connect(ui->actionClose, &QAction::triggered, this, [&]{closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_Application, &QAction::triggered, this, &MainWindow::displayAboutApplication);
    connect(ui->actionRotateLeft, &QAction::triggered, this, &MainWindow::rotateImageLeft);
    connect(ui->actionRotate_Right, &QAction::triggered, this, &MainWindow::rotateImageRight);
    connect(ui->rotateRightButton, &QPushButton::clicked, this, &MainWindow::rotateImageRight);
    connect(ui->rotateLeftButton, &QPushButton::clicked, this, &MainWindow::rotateImageLeft);
    connect(ui->actionProperties, &QAction::triggered, this, &MainWindow::displayFileProperties);
    connect(ui->horizontalSlider, &QSlider::actionTriggered, this, &MainWindow::zoom);
    connect(ui->actionNext, &QAction::triggered, this, &MainWindow::openImageRight);
    connect(ui->actionClearRecent, &QAction::triggered, this, &MainWindow::clearRecent);
    connect(ui->actionPrevious, &QAction::triggered, this, &MainWindow::openImegeLeft);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);

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

void MainWindow::openImageInCurrentTab(const QString& path){
    QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->currentWidget());
    if(widget != nullptr){
        ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
        if(imageView != nullptr)
        {
            QImage image;
            if(image.load(path)){
                setVisibility(true);
                imagePathLabel->setText(path);
                saveToRecent(path);
                imageView->setImage(image, path);
                ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(path).fileName());
            }
        }
    }
}

void MainWindow::displayAboutApplication(){
    QMessageBox::about(this, tr("About Application"), tr(R"(
    <html>
        <body>
            <h2>Image Viewer</h2>
            <p>PCS Image Viewer is a simple image viewer. It is a part of the <a href="%3">Pinecone Snake project</a>.</p>
            <p>Version: %1</p>
            <p>License: <a href="%2">GPL 3</a></p>
        </body>
    </html>
    )").arg(VERSION, LICENSELINK, PROJECTLINK));
}

void MainWindow::rotateImageRight(){
    QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->currentWidget());
    if(widget != nullptr){
        ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
        if(imageView != nullptr)
        {
            imageView->rotateRight();
        }
    }
}

void MainWindow::rotateImageLeft()
{
    QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->currentWidget());
    if(widget != nullptr){
        ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
        if(imageView != nullptr)
        {
            imageView->rotateLeft();
        }
    }
}

void MainWindow::displayFileProperties(){
    QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->currentWidget());
    if(widget != nullptr){
        ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
        if(imageView != nullptr){
            PropertiesDialog dialog(imageView->getPath(), this);
            dialog.exec();
        }
    }
}

void MainWindow::zoom(){
    QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->currentWidget());
    if(widget != nullptr){
        ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
        if(imageView != nullptr)
        {
            int newPos = ui->horizontalSlider->value();
            if(newPos > sliderPosition)
            {
                imageView->setScale(1.1);
            }
            else
            {
                imageView->setScale(0.9);
            }
            sliderPosition = newPos;
        }
    }
}

void MainWindow::saveToRecent(const QString& path)
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
        if(!paths.contains(path))
        {
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            if(file.isOpen())
            {
                QTextStream stream(&file);
                if(paths.length() > 10) paths.removeAt(0);
                paths.append(path);
                for(const auto& i : std::as_const(paths))
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
        setVisibility(true);
        imagePathLabel->setText(path);
        saveToRecent(path);
        QScrollArea* scrollArea = new QScrollArea();
        ImageView* imageView = new ImageView();
        imageView->setImage(image, path);
        scrollArea->setWidget(imageView);
        imageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        scrollArea->setMinimumSize(0, 0);
        scrollArea->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        scrollArea->setWidgetResizable(true);
        connect(imageView, &ImageView::scaleChanged, this, &MainWindow::changeScaleSlider);
        ui->tabWidget->addTab(scrollArea, QFileInfo(path).fileName());
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    }
}

void MainWindow::openImegeLeft()
{
    QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->currentWidget());
    if(widget != nullptr){
        ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
        if(imageView != nullptr){
            QFileInfoList images = getImagesInDirectory(imageView->getPath());
            int index = images.indexOf(QFileInfo(imageView->getPath()));
            if(index < 0)
                return;
            if(index == 0){
                openImageInCurrentTab(images[images.length()-1].absoluteFilePath());
            }
            else{
                openImageInCurrentTab(images[index-1].absoluteFilePath());
            }
        }
    }
}

void MainWindow::openImageRight()
{
    QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->currentWidget());
    if(widget != nullptr){
        ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
        if(imageView != nullptr){
            QFileInfoList images = getImagesInDirectory(imageView->getPath());
            int index = images.indexOf(QFileInfo(imageView->getPath()));
            if(index < 0)
                return;
            if(index >= images.size() || index + 1 >= images.size()){
                openImageInCurrentTab(images[0].absoluteFilePath());
            }
            else{
                openImageInCurrentTab(images[index+1].absoluteFilePath());
            }
        }
    }
}

QFileInfoList MainWindow::getImagesInDirectory(const QString& path)
{
    QString dirPath = QFileInfo(path).absolutePath();
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
    if(!QFileInfo::exists(path))
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

void MainWindow::closeTab(int index)
{
    QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->widget(index));
    if(widget != nullptr)
    {
        ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
        if(imageView != nullptr){
            delete imageView;
        }
    }
    ui->tabWidget->removeTab(index);
}

void MainWindow::tabChanged(int index)
{
    if(ui->tabWidget->count() <= 0)
    {
        setVisibility(false);
        imagePathLabel->setText("");
    }
    else
    {
        QScrollArea* widget = dynamic_cast<QScrollArea*>(ui->tabWidget->widget(index));
        if(widget != nullptr)
        {
            ImageView* imageView = dynamic_cast<ImageView*>(widget->widget());
            if(imageView != nullptr)
            {
                imagePathLabel->setText(imageView->getPath());
                ui->horizontalSlider->setValue(imageView->getScale());
            }
        }
    }
}
