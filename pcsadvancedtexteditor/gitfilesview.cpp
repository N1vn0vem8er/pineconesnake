#include "gitfilesview.h"
#include <qmenu.h>

GitFilesView::GitFilesView(QWidget *parent) : QTableView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &GitFilesView::customContextMenuRequested, this, &GitFilesView::openContextMenu);
}

void GitFilesView::setContextMenu(QMenu *contextMenu)
{
    this->contextMenu = contextMenu;
}

void GitFilesView::gitAddPressed()
{

    emit gitAdd(indexAt(currentPoint));
}

void GitFilesView::gitDiffPressed()
{
    emit gitDiff(indexAt(currentPoint));
}

void GitFilesView::openContextMenu(const QPoint& point)
{
    assert((contextMenu != nullptr) && "Context menu cannot be nullptr");
    const auto index = indexAt(point);
    currentPoint = point;
    contextMenu->popup(mapToGlobal(point));
}
