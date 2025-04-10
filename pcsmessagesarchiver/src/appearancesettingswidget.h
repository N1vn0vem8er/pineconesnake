#ifndef APPEARANCESETTINGSWIDGET_H
#define APPEARANCESETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class AppearanceSettingsWidget;
}

class AppearanceSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AppearanceSettingsWidget(QWidget *parent = nullptr);
    ~AppearanceSettingsWidget();

private:
    Ui::AppearanceSettingsWidget *ui;
private slots:
    void apply();
};

#endif // APPEARANCESETTINGSWIDGET_H
