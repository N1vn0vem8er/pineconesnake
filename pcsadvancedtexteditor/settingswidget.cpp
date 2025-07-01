#include "settingswidget.h"
#include "ui_settingswidget.h"
#include <qformlayout.h>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(1, 1);
    categoriesModel = new QStringListModel(categories, ui->listView);
    ui->listView->setModel(categoriesModel);
    connect(ui->listView, &QListView::doubleClicked, this, &SettingsWidget::pageChanged);
    initGeneralSettings();
    initAppearanceSettings();
    initSpellCheckingSettings();
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::initGeneralSettings()
{
    QFormLayout* layout = new QFormLayout(ui->generalPage);
}

void SettingsWidget::initAppearanceSettings()
{
    QFormLayout* layout = new QFormLayout(ui->appearancePage);
}

void SettingsWidget::initSpellCheckingSettings()
{
    QFormLayout* layout = new QFormLayout(ui->spellcheckingPage);
    defaultLanguageComboBox = new QComboBox(ui->spellcheckingPage);
    layout->addRow(tr("Default language"), defaultLanguageComboBox);
}

void SettingsWidget::pageChanged(const QModelIndex& index)
{
    if(categories.size() > index.row())
    {
        ui->stackedWidget->setCurrentIndex(index.row());
    }
}
