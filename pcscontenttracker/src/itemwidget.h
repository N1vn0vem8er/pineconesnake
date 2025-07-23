#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include "structs.h"

#include <QWidget>

namespace Ui {
class ItemWidget;
}

class ItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget(Item item, QWidget *parent = nullptr);
    ~ItemWidget();
public slots:
    void deletePressed();
    void editPressed();
signals:
    void deleteItem(const Item& item);
    void editItem(Item& item);
    void requestRefresh();
private:
    Ui::ItemWidget *ui;
    Item item;
    QList<Status> states;
private slots:
    void open();
    void start();
    void finish();
    void rewatch();
    void increment();
};

#endif // ITEMWIDGET_H
