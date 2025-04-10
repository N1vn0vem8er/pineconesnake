#ifndef BROWSEMESSAGESWIDGET_H
#define BROWSEMESSAGESWIDGET_H

#include "globals.h"
#include <QStringListModel>
#include <QWidget>

namespace Ui {
class BrowseMessagesWidget;
}

class BrowseMessagesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrowseMessagesWidget(QWidget *parent = nullptr);
    void refresh();
    void openById(int id);
    ~BrowseMessagesWidget();

private:
    Ui::BrowseMessagesWidget *ui;
    QStringList messagesList;
    QStringListModel* model = nullptr;
    QList<Globals::Message> messages;
    Globals::Message openedMessage;
    void setInfoVisible(bool val);
private slots:
    void openSelected();
    void save();
    void deleteMessage();
};

#endif // BROWSEMESSAGESWIDGET_H
