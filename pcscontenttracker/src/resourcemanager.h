#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "structs.h"
#include <sqlite3.h>
class ResourceManager
{
public:
    static ResourceManager *getInstance();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    void close() const;
    QList<Item> getAllItems() const;
    QList<Status> getAllStates() const;
    void addItem(const Item& item) const;
    void deleteItem(const Item& item) const;
    void editItem(const Item& item) const;
    QList<Item> getAllFinished() const;
    QList<Item> getAllCurrnet() const;
    QList<Item> getAllPlanned() const;
    void setStatus(int itemId, int statusId) const;
    void setChapter(int id, int val) const;
    QList<Item> get10ItemsByTitle(const QString& title) const;
private:
    ResourceManager();
    ~ResourceManager();
    sqlite3* database = nullptr;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCEMANAGER_H
