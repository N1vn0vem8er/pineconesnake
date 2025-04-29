#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "qstringlistmodel.h"
#include <QWidget>

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
    QStringList settingsCategories = {tr("Appearance"), tr("Database")};
    QStringListModel* model;
private slots:
    void openSettingsPage(const QModelIndex &index);
};

#endif // SETTINGSWIDGET_H
