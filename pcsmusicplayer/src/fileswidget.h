#ifndef FILESWIDGET_H
#define FILESWIDGET_H

#include "fileslistitemwidget.h"
#include "fileslistmodel.h"
#include <QWidget>

namespace Ui {
class FilesWidget;
}

class FilesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FilesWidget(QWidget *parent = nullptr);
    ~FilesWidget();
    void loadFiles();

private:
    Ui::FilesWidget *ui;
    FilesListItemWidget* delegate = nullptr;
    FilesListModel* model = nullptr;
    QList<Track> tracks;
};

#endif // FILESWIDGET_H
