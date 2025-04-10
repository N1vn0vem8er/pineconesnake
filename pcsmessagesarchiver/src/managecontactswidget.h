#ifndef MANAGEPEOPLEWIDGET_H
#define MANAGEPEOPLEWIDGET_H

#include "globals.h"
#include <QStringListModel>
#include <QWidget>

namespace Ui {
class ManageContactsWidget;
}

class ManageContactsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ManageContactsWidget(QWidget *parent = nullptr);
    ~ManageContactsWidget();

private:
    Ui::ManageContactsWidget *ui;
    QStringListModel* model = nullptr;
    void refresh();
    QList<Globals::Contact> people;
private slots:
    void deleteSelected();
    void addNew();
};

#endif // MANAGEPEOPLEWIDGET_H
