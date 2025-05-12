#include "albumitemwidget.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "resourcesmanager.h"
#include <QMouseEvent>
#include <QPushButton>

AlbumItemWidget::AlbumItemWidget(const QString title, QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QLabel* icon = new QLabel(this);
    icon->setPixmap(QIcon::fromTheme("folder").pixmap(QSize(64, 64)));
    QLabel* titleLabel = new QLabel(this);
    titleLabel->setText(title);
    QPushButton* button = new QPushButton(this);
    button->setIcon(QIcon::fromTheme("list-add"));
    connect(button, &QPushButton::clicked, this, &AlbumItemWidget::buttonClicked);
    layout->addWidget(icon);
    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(button);
    setLayout(layout);
    this->title = title;
}

void AlbumItemWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit doubleClicked(title);
}

void AlbumItemWidget::buttonClicked()
{
    emit addToPlaylist(ResourcesManager::getInstance()->getAllTrackInAlbum(title));
}
