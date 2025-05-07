#ifndef FAVORITEWIDGET_H
#define FAVORITEWIDGET_H

#include "favoriteitemwidget.h"
#include "favoritemodel.h"
#include <QWidget>

namespace Ui {
class FavoriteWidget;
}

class FavoriteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteWidget(QWidget *parent = nullptr);
    ~FavoriteWidget();
    void loadFavorites();

private:
    Ui::FavoriteWidget *ui;
    FavoriteItemWidget* delegate = nullptr;
    FavoriteModel* model = nullptr;
};

#endif // FAVORITEWIDGET_H
