#include "eventmanager.h"
#include "mainwindow.h"
#include "qapplication.h"
#include "resourcesmanager.h"

#include <QMenu>
#include <QTimer>

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
    connect(qApp, &QApplication::aboutToQuit, this, []{ResourcesManager::getInstance()->close();});
    icon->setContextMenu(menu);
    icon->show();
    refresh();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EventManager::checkEvents);
    timer->setInterval(1000);
    timer->start();
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

void EventManager::checkEvents()
{
    for(auto& i : repeatedEvents)
    {
        i.second--;
        if(i.second <= 0)
        {
            i.second = i.first.everySeconds;
        }
    }
}

void EventManager::refresh()
{
    ResourcesManager* rm = ResourcesManager::getInstance();
    for(const auto& i : rm->getAllRepeating())
    {
        repeatedEvents.append(QPair(i, i.everySeconds));
    }
    events = rm->getAllEvents();
}
