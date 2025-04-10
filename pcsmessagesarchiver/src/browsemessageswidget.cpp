#include "browsemessageswidget.h"
#include "resourcesmanager.h"
#include "settings.h"
#include "ui_browsemessageswidget.h"

#include <QMessageBox>

BrowseMessagesWidget::BrowseMessagesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BrowseMessagesWidget)
{
    ui->setupUi(this);
    connect(ui->listView, &QListView::doubleClicked, this, &BrowseMessagesWidget::openSelected);
    connect(ui->widget, &Writer::save, this, &BrowseMessagesWidget::save);
    connect(ui->deleteButton, &QPushButton::clicked, this, &BrowseMessagesWidget::deleteMessage);
    setInfoVisible(false);
    refresh();
    ui->splitter->setStretchFactor(1, Settings::browserSplitterStretchFactor);
}

void BrowseMessagesWidget::refresh()
{
    ResourcesManager* rm = ResourcesManager::getInstance();
    if(model != nullptr)
    {
        delete model;
    }
    messagesList.clear();
    model = new QStringListModel(this);
    messages = rm->getAllMessages();
    for(const auto& i : messages)
    {
        messagesList.append(QString("%1\t%2\t%3\t%4\t%5").arg(i.title).arg(i.from).arg(i.to).arg(i.created).arg(i.modified));
    }
    model->setStringList(messagesList);
    ui->listView->setModel(model);
}

void BrowseMessagesWidget::openById(int id)
{
    ResourcesManager* rm = ResourcesManager::getInstance();
    Globals::Message msg = rm->getMessageById(id);
    ui->widget->setTitle(msg.title);
    ui->widget->setFrom(msg.from);
    ui->widget->setContent(msg.contents);
    ui->widget->setTo(msg.to);
    setInfoVisible(true);
    openedMessage = msg;
}

BrowseMessagesWidget::~BrowseMessagesWidget()
{
    delete ui;
}

void BrowseMessagesWidget::setInfoVisible(bool val)
{
    ui->label_3->setVisible(!val);
    ui->widget->setVisible(val);
    ui->deleteButton->setVisible(val);
}

void BrowseMessagesWidget::openSelected()
{
    ResourcesManager* rm = ResourcesManager::getInstance();
    Globals::Message msg = rm->getMessageById(messages[ui->listView->selectionModel()->selectedIndexes()[0].row()].id);
    ui->widget->setTitle(msg.title);
    ui->widget->setFrom(msg.from);
    ui->widget->setContent(msg.contents);
    ui->widget->setTo(msg.to);
    setInfoVisible(true);
    openedMessage = msg;
}

void BrowseMessagesWidget::save()
{
    openedMessage.contents = ui->widget->getContent();
    openedMessage.title = ui->widget->getTitle();
    openedMessage.from = ui->widget->getFrom();
    openedMessage.to = ui->widget->getTo();
    QDateTime dt = QDateTime::currentDateTime();
    openedMessage.modified = dt.toString("yyyy-MM-dd HH:mm:ss");
    ResourcesManager* rm = ResourcesManager::getInstance();
    rm->updateMessage(openedMessage);
    refresh();
}

void BrowseMessagesWidget::deleteMessage()
{
    QMessageBox::StandardButton dialog = QMessageBox::question(this, tr("Are you sure?"), tr("Delete message?"), QMessageBox::Yes | QMessageBox::No);
    if(dialog == QMessageBox::Yes)
    {
        ResourcesManager* rm = ResourcesManager::getInstance();
        rm->deleteMessageById(messages[ui->listView->selectionModel()->selectedIndexes()[0].row()].id);
        refresh();
        setInfoVisible(false);
    }
}
