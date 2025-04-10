#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    model = new QStringListModel(ui->listView);
    model->setStringList(settingsCategories);
    ui->listView->setModel(model);
    connect(ui->listView, &QListView::doubleClicked, this, &SettingsWidget::openSettingsPage);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::openSettingsPage(const QModelIndex& index)
{
    ui->stackedWidget->setCurrentIndex(index.row());
}
