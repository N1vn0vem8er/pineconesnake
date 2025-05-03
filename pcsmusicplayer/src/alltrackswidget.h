#ifndef ALLTRACKSWIDGET_H
#define ALLTRACKSWIDGET_H

#include "trackitemwidget.h"
#include "tracklistmodel.h"
#include <QWidget>

namespace Ui {
class AllTracksWidget;
}

class AllTracksWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AllTracksWidget(QWidget *parent = nullptr);
    ~AllTracksWidget();

private:
    Ui::AllTracksWidget *ui;
    TrackItemWidget* delegate = nullptr;
    TrackListModel* model = nullptr;
};

#endif // ALLTRACKSWIDGET_H
