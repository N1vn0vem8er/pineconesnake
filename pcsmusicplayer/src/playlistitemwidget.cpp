#include "playlistitemwidget.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"

PlaylistItemWidget::PlaylistItemWidget(const QString &name, QWidget *parent)
    : QWidget{parent}
{
    this->name = name;
    QHBoxLayout* layout = new QHBoxLayout(this);
    QLabel* icon = new QLabel(this);
    icon->setPixmap(QIcon::fromTheme("folder").pixmap(QSize(64, 64)));
    QLabel* titleLabel = new QLabel(this);
    titleLabel->setText(this->name);
    QPushButton* button = new QPushButton(this);
    button->setIcon(QIcon::fromTheme("media-playback-start"));
    connect(button, &QPushButton::clicked, this, &PlaylistItemWidget::playPressed);
    QPushButton* buttonDelete = new QPushButton(this);
    buttonDelete->setIcon(QIcon::fromTheme("edit-delete"));
    connect(buttonDelete, &QPushButton::clicked, this, &PlaylistItemWidget::deletePlaylist);
    layout->addWidget(icon);
    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(button);
    layout->addWidget(buttonDelete);
    setLayout(layout);
}

void PlaylistItemWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit doubleClicked(name);
}

void PlaylistItemWidget::playPressed()
{
    emit play(name);
}

void PlaylistItemWidget::deletePlaylist()
{
    emit del(name);
}
