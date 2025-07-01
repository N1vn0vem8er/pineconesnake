#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <qstringlistmodel.h>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

private:
    Ui::SettingsWidget *ui;
    QStringList categories {tr("General"), tr("Appearance"), tr("Spell Checking")};
    QStringListModel* categoriesModel {nullptr};
    QComboBox* defaultLanguageComboBox {nullptr};
    void initGeneralSettings();
    void initAppearanceSettings();
    void initSpellCheckingSettings();

private slots:
    void pageChanged(const QModelIndex& index);
};

#endif // SETTINGSWIDGET_H
