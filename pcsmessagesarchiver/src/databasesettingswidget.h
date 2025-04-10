#ifndef DATABASESETTINGSWIDGET_H
#define DATABASESETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class DatabaseSettingsWidget;
}

class DatabaseSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseSettingsWidget(QWidget *parent = nullptr);
    ~DatabaseSettingsWidget();

private:
    Ui::DatabaseSettingsWidget *ui;
private slots:
    void apply();
    void selectPath();
    void exportDatabaseToFile();
    void exportPeopleToFile();
};

#endif // DATABASESETTINGSWIDGET_H
