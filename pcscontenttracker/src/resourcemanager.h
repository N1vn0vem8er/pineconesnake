#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "structs.h"
#include <mutex>
#include <sqlite3.h>
class ResourceManager
{
public:
    static ResourceManager *getInstance();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    void close();
    QList<Item> getAllItems();
    QList<Status> getAllStates();
    void addItem(const Item& item);
    void deleteItem(const Item& item);
    void editItem(const Item& item);
    QList<Item> getAllFinished();
    QList<Item> getAllCurrnet();
    QList<Item> getAllPlanned();
    void setStatus(int itemId, int statusId);
    void setChapter(int id, int val);
    QList<Item> get10ItemsByTitle(const QString& title);
private:
    ResourceManager();
    ~ResourceManager();
    sqlite3* database = nullptr;
    static std::mutex mutex;
    static ResourceManager* instancePtr;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCEMANAGER_H
