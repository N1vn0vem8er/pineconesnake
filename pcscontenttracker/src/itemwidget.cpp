#include "itemwidget.h"
#include "resourcemanager.h"
#include "settings.h"
#include "ui_itemwidget.h"

#include <QFileInfo>
#include <QProcess>
#include <qpixmap.h>

ItemWidget::ItemWidget(Item item, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ItemWidget)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &ItemWidget::open);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ItemWidget::deletePressed);
    connect(ui->editButton, &QPushButton::clicked, this, &ItemWidget::editPressed);
    connect(ui->startButton, &QPushButton::clicked, this, &ItemWidget::start);
    connect(ui->finishButton, &QPushButton::clicked, this, &ItemWidget::finish);
    connect(ui->rewatchButton, &QPushButton::clicked, this, &ItemWidget::rewatch);
    connect(ui->incrementButton, &QPushButton::clicked, this, &ItemWidget::increment);
    this->item = item;
    ui->rewatchButton->setVisible(false);
    ui->startButton->setVisible(false);
    QString status = "";
    states = ResourceManager::getInstance()->getAllStates();
    for(const auto& i : std::as_const(states))
    {
        if(i.id == item.status)
        {
            status = i.name;
            if(i.name == "Planned")
            {
                ui->startButton->setVisible(true);
            }
            else if(i.name == "Current")
            {

            }
            else if(i.name == "Finished")
            {
                ui->rewatchButton->setVisible(true);
                ui->finishButton->setVisible(false);
            }
        }
    }
    const QString imageAbsolutePath = Settings::imagesPath + "/" + item.image;
    if(QFileInfo::exists(imageAbsolutePath))
    {
        QImage image;
        image.load(imageAbsolutePath);
        if(!image.isNull())
        {
            ui->imageLabel->setPixmap(QPixmap::fromImage(image.scaled(QSize(150, 200))));
        }
    }
    ui->label->setText(tr("<html><body><h1>%1</h1><h3>Status: %2\tChapter: %3/%4</h3><p>%5</p></body></html>").arg(item.title, status).arg(item.chapter).arg(item.chapters).arg(item.description));
}

ItemWidget::~ItemWidget()
{
    delete ui;
}

void ItemWidget::open()
{
    QProcess* process = new QProcess();
    process->execute("xdg-open", QStringList() << item.link);
    process->waitForStarted();
    process->waitForFinished();
    delete process;
}

void ItemWidget::deletePressed()
{
    emit deleteItem(item);
}

void ItemWidget::editPressed()
{
    emit editItem(item);
}

void ItemWidget::start()
{
    for(const auto& i : std::as_const(states))
    {
        if(i.name == "Current")
        {
            ResourceManager::getInstance()->setStatus(item.id, i.id);
            break;
        }
    }
    emit requestRefresh();
}

void ItemWidget::finish()
{
    for(const auto& i : std::as_const(states))
    {
        if(i.name == "Finished")
        {
            ResourceManager::getInstance()->setStatus(item.id, i.id);
            ResourceManager::getInstance()->setChapter(item.id, item.chapters);
            break;
        }
    }
    emit requestRefresh();
}

void ItemWidget::rewatch()
{
    for(const auto& i : std::as_const(states))
    {
        if(i.name == "Planned")
        {
            ResourceManager::getInstance()->setStatus(item.id, i.id);
            ResourceManager::getInstance()->setChapter(item.id, 0);
            break;
        }
    }
    emit requestRefresh();
}

void ItemWidget::increment()
{
    item.chapter++;
    int ch = item.chapter;
    if(ch > item.chapters)
        finish();
    else
        ResourceManager::getInstance()->setChapter(item.id, ch);
    emit requestRefresh();
}
