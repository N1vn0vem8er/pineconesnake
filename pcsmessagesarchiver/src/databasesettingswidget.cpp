#include "databasesettingswidget.h"
#include "resourcesmanager.h"
#include "settings.h"
#include "ui_databasesettingswidget.h"
#include <QDir>
#include <QFileDialog>

DatabaseSettingsWidget::DatabaseSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DatabaseSettingsWidget)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QPushButton::clicked, this, &DatabaseSettingsWidget::apply);
    connect(ui->selectPathButton, &QPushButton::clicked, this, &DatabaseSettingsWidget::selectPath);
    connect(ui->exportButton, &QPushButton::clicked, this, &DatabaseSettingsWidget::exportDatabaseToFile);
    connect(ui->ptoFileButton, &QPushButton::clicked, this, &DatabaseSettingsWidget::exportPeopleToFile);
    ui->dbLocationLine->setText(Settings::databasePath);
}

DatabaseSettingsWidget::~DatabaseSettingsWidget()
{
    delete ui;
}

void DatabaseSettingsWidget::apply()
{
    if(QDir(ui->dbLocationLine->text()).exists())
    {
        Settings::databasePath = ui->dbLocationLine->text();
        Settings settings;
        settings.saveSettings();
    }
}

void DatabaseSettingsWidget::selectPath()
{
    const QString path = QFileDialog::getExistingDirectory(this, "Select Directory", Settings::databasePath);
    if(!path.isEmpty())
    {
        ui->dbLocationLine->setText(path);
    }
}

void DatabaseSettingsWidget::exportDatabaseToFile()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Export Database"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        if(file.isOpen())
        {
            QTextStream stream(&file);
            stream << "id,title,contents,sender,reciver,created,modified\n";
            ResourcesManager* rm = ResourcesManager::getInstance();
            for(const auto& i : rm->getAllMessages())
            {
                stream << QString("%1,%2,%3,%4,%5,%6,%7\n").arg(i.id).arg(i.title).arg(i.contents).arg(i.from).arg(i.to).arg(i.created).arg(i.modified);
            }
            file.close();
        }
    }
}

void DatabaseSettingsWidget::exportPeopleToFile()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Export People"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        if(file.isOpen())
        {
            QTextStream stream(&file);
            stream << "id,name\n";
            ResourcesManager* rm = ResourcesManager::getInstance();
            for(const auto& i : rm->getAllContacts())
            {
                stream << QString("%1,%2\n").arg(i.id).arg(i.name);
            }
            file.close();
        }
    }
}
