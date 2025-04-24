#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "structs.h"
#include <mutex>
#include <sqlite3.h>


class ResourcesManager
{
public:
public:
    static ResourcesManager *getInstance();
    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    void close();
    QList<Note> getAllNotes();
    QList<Note> getNotesForTitle(const QString title);
    void addNote(const Note& note);
private:
    ResourcesManager();
    ~ResourcesManager();
    sqlite3* database = nullptr;
    static std::mutex mutex;
    static ResourcesManager* instancePtr;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCESMANAGER_H
