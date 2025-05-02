#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "structs.h"
#include <mutex>
#include <sqlite3.h>
class ResourcesManager
{
public:
    static ResourcesManager *getInstance();
    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    void close();
    void saveTracks(QList<Track> tracks);
    QStringList getAllPaths();
    QList<Track> getAllTracks();

private:
    ResourcesManager();
    ~ResourcesManager();
    sqlite3* database = nullptr;
    static std::mutex mutex;
    static ResourcesManager* instancePtr;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCESMANAGER_H
