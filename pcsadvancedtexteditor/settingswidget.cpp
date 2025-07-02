#include "settingswidget.h"
#include "globals.h"
#include "ui_settingswidget.h"
#include "settings.h"
#include <QCheckBox>
#include <qformlayout.h>
#include <qlineedit.h>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QPushButton::clicked, this, &SettingsWidget::apply);
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
    ui->generalPage->setLayout(layout);
}

void SettingsWidget::initAppearanceSettings()
{
    QFormLayout* layout = new QFormLayout(ui->appearancePage);
    fontSize = new QLineEdit(ui->appearancePage);
    fontSize->setInputMask("99");
    fontSize->setText(QString::number(Settings::defaultFontSize));
    layout->addRow(tr("Default font size"), fontSize);
    ui->appearancePage->setLayout(layout);
}

void SettingsWidget::initSpellCheckingSettings()
{
    QFormLayout* layout = new QFormLayout(ui->spellcheckingPage);
    defaultLanguageComboBox = new QComboBox(ui->spellcheckingPage);
    for(int i = 0; i<Globals::hunspellLanguages.size(); i+=2)
    {
        defaultLanguageComboBox->addItem(Globals::hunspellLanguages[i].left(Globals::hunspellLanguages[i].indexOf(".")));
    }
    defaultLanguageComboBox->setCurrentIndex(defaultLanguageComboBox->findText(Settings::defaultLanguage));
    layout->addRow(tr("Default language"), defaultLanguageComboBox);
    hunspellDirs = new QLineEdit(ui->spellcheckingPage);
    QString dirs {""};
    for(const auto& i : std::as_const(Settings::hunspellDirs))
    {
        dirs += i + ';';
    }
    hunspellDirs->setText(dirs);
    layout->addRow(tr("Dictionary dirs. Spearated by \';\'"), hunspellDirs);
    spellCheckEnabled = new QCheckBox(ui->spellcheckingPage);
    spellCheckEnabled->setChecked(Settings::spellCheckEnabled);
    layout->addRow(tr("Spellcheck enabled for new editors"), spellCheckEnabled);
}

void SettingsWidget::pageChanged(const QModelIndex& index)
{
    if(categories.size() > index.row())
    {
        ui->stackedWidget->setCurrentIndex(index.row());
    }
}

void SettingsWidget::apply()
{
    switch(ui->stackedWidget->currentIndex())
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
    {
        Settings::defaultLanguage = defaultLanguageComboBox->currentText();
        Settings::hunspellDirs = hunspellDirs->text().split(';');
        Settings::hunspellDirs.removeAll("");
        Settings::spellCheckEnabled = spellCheckEnabled->isChecked();
        emit reloadHunspell();
    }
        break;
    }
}
