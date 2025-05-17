#include "settings.h"
#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QPushButton::clicked, this, &SettingsWidget::apply);
    QString paths = "";
    for(const auto& i : Settings::searchDirs)
    {
        paths.append(i).append(';');
    }
    ui->searchDirLine->setText(paths);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::apply()
{
    Settings s;
    QStringList dirs;
    QString path = "";
    for(const auto& i : ui->searchDirLine->text())
    {
        if(i == ' ') continue;
        if(i == ';')
        {
            dirs.append(path);
            path.clear();
            continue;
        }
        path+=i;
    }
    Settings::searchDirs = dirs;
    s.saveSettings();
}
