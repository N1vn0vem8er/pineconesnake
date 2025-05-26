#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "eventmanager.h"
#include <mutex>
#include <sqlite3.h>
#include <QString>


class ResourcesManager
{
public:
    static ResourcesManager *getInstance();
    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    void close();
    void saveEvent(const EventManager::Event& event);
    void saveRepeating(const EventManager::RepeatedEvent& event);
    QList<EventManager::RepeatedEvent> getAllRepeating();
    QList<EventManager::Event> getAllEvents();
private:
    ResourcesManager();
    ~ResourcesManager();
    sqlite3* database = nullptr;
    static std::mutex mutex;
    static ResourcesManager* instancePtr;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCESMANAGER_H
