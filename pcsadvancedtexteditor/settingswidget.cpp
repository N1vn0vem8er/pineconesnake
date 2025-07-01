#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}
