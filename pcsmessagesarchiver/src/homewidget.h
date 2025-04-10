#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include "globals.h"
#include "qstringlistmodel.h"
#include <QWidget>

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();
signals:
    void openMessageFromRecent(int id);
private:
    Ui::HomeWidget *ui;
    QStringListModel* model;
    QList<Globals::Message> recent;
private slots:
    void recentClicked(const QModelIndex& index);
};

#endif // HOMEWIDGET_H
