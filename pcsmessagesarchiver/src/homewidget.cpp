#include "homewidget.h"
#include "resourcesmanager.h"
#include "ui_homewidget.h"

HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
    connect(ui->listView, &QListView::doubleClicked, this, &HomeWidget::recentClicked);
    ui->welcomeLabel->setText(ui->welcomeLabel->text() + " " + (qEnvironmentVariableIsEmpty("USER") ? qgetenv("USERNAME") : qgetenv("USER")));
    ResourcesManager* rm = ResourcesManager::getInstance();
    recent = rm->getRecentMessages();
    QStringList recentMessages;
    for(const auto& i : std::as_const(recent))
    {
        recentMessages.append(i.title);
    }
    model = new QStringListModel(ui->listView);
    model->setStringList(recentMessages);
    ui->listView->setModel(model);
}

HomeWidget::~HomeWidget()
{
    delete ui;
}

void HomeWidget::recentClicked(const QModelIndex &index)
{
    emit openMessageFromRecent(recent[index.row()].id);
}
