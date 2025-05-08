#include "albumitemwidget.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include <QMouseEvent>

AlbumItemWidget::AlbumItemWidget(const QString title, QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* icon = new QLabel(this);
    icon->setPixmap(QIcon::fromTheme("folder").pixmap(QSize(64, 64)));
    QLabel* titleLabel = new QLabel(this);
    titleLabel->setText(title);
    layout->addWidget(icon);
    layout->addWidget(titleLabel);
    setLayout(layout);
    this->title = title;
}

void AlbumItemWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit doubleClicked(title);
}
