#include "artistitemwidget.h"

#include <QHBoxLayout>
#include <QLabel>

ArtistItemWidget::ArtistItemWidget(const QString &title, QWidget *parent)
    : QWidget{parent}
{
    this->title = title;
    QLabel* label = new QLabel(this);
    label->setText(this->title);
    QLabel* img = new QLabel(this);
    img->setPixmap(QIcon::fromTheme("user-bookmarks").pixmap(QSize(64,64)));
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(img);
    layout->addWidget(label);
    setLayout(layout);
}

void ArtistItemWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked(title);
}
