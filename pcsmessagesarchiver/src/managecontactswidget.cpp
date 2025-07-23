#include "managecontactswidget.h"
#include "resourcesmanager.h"
#include "ui_managecontactswidget.h"

ManageContactsWidget::ManageContactsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ManageContactsWidget)
{
    ui->setupUi(this);
    connect(ui->addNewButton, &QPushButton::clicked, this, &ManageContactsWidget::addNew);
    connect(ui->deleteSelectedButton, &QPushButton::clicked, this, &ManageContactsWidget::deleteSelected);
    refresh();
}

ManageContactsWidget::~ManageContactsWidget()
{
    delete ui;
}

void ManageContactsWidget::refresh()
{
    if(model!=nullptr)
    {
        delete model;
    }
    QStringList names;
    people = ResourcesManager::getInstance()->getAllContacts();
    for(const auto& i : std::as_const(people))
    {
        names.append(i.name);
    }
    model = new QStringListModel(ui->listView);
    model->setStringList(names);
    ui->listView->setModel(model);
}

void ManageContactsWidget::deleteSelected()
{
    QList<Globals::Contact> toDelete;
    const auto selected = ui->listView->selectionModel()->selectedIndexes();
    for(const auto& i : selected)
    {
        toDelete.append(people[i.row()]);
    }
    ResourcesManager* rm = ResourcesManager::getInstance();
    for(const auto& i : toDelete)
    {
        rm->deleteContactById(i.id);
    }
    refresh();
}

void ManageContactsWidget::addNew()
{
    const QString name = ui->lineEdit->text();
    if(!name.isEmpty())
    {
        ResourcesManager::getInstance()->saveContactInNotExists(name);
    }
    refresh();
}
