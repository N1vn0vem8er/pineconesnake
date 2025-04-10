#include "appearancesettingswidget.h"
#include "settings.h"
#include "ui_appearancesettingswidget.h"

#include <QStyleFactory>

AppearanceSettingsWidget::AppearanceSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AppearanceSettingsWidget)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QPushButton::clicked, this, &AppearanceSettingsWidget::apply);
    ui->mainSF->setText(QString::number(Settings::mainSplitterStretchFactor));
    ui->browserSF->setText(QString::number(Settings::browserSplitterStretchFactor));
    ui->themeComboBox->addItems(QStyleFactory::keys());
    if(QStyleFactory::keys().contains(Settings::theme))
    {
        ui->themeComboBox->setCurrentText(Settings::theme);
    }
}

AppearanceSettingsWidget::~AppearanceSettingsWidget()
{
    delete ui;
}

void AppearanceSettingsWidget::apply()
{
    Settings::mainSplitterStretchFactor = ui->mainSF->text().toInt();
    Settings::browserSplitterStretchFactor = ui->browserSF->text().toInt();
    Settings::theme = ui->themeComboBox->currentText();
    QApplication::setStyle(Settings::theme);
    Settings s;
    s.saveSettings();
}
