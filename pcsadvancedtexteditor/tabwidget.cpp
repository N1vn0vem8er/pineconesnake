#include "tabwidget.h"
#include <QPainter>

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
{
    setTabsClosable(true);
}

void TabWidget::paintEvent(QPaintEvent *event)
{
    QTabWidget::paintEvent(event);
    if(!count())
    {
        QPainter painter(this);
        painter.setPen(QColor::fromRgb(134, 134, 134));
        painter.drawText(rect(), Qt::AlignCenter, tr("CTRL + N - New File\n\nCTRL + O - Open File"));
    }
}
