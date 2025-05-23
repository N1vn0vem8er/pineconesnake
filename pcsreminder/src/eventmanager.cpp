#include "eventmanager.h"
#include "mainwindow.h"
#include "qapplication.h"

#include <QMenu>

EventManager::EventManager(QObject *parent)
    : QObject{parent}
{
    icon = new QSystemTrayIcon(this);
    icon->setIcon(QIcon::fromTheme("media-record")); //placeholder replace
    menu = new QMenu();
    QAction* settingsAction = menu->addAction(tr("Settings"));
    connect(settingsAction, &QAction::triggered, this, &EventManager::openSettings);
    QAction* closeAction = menu->addAction(tr("Close"));
    connect(closeAction, &QAction::triggered, this, &EventManager::close);
    icon->setContextMenu(menu);
    icon->show();
}

EventManager::~EventManager()
{
    delete menu;
}

void EventManager::close()
{
    qApp->quit();
}

void EventManager::openSettings()
{
    MainWindow* w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}
