#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "structs.h"
#include <sqlite3.h>


class ResourcesManager
{
public:
    static ResourcesManager *getInstance();
    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    void close();
    QList<Note> getAllNotes();
    QList<Note> getNotesForTitle(const QString &title);
    int getLastId();
    void addNote(const Note& note);
    void editNote(const Note& note);
    void deleteNote(const Note& note);
private:
    ResourcesManager();
    ~ResourcesManager();
    sqlite3* database = nullptr;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCESMANAGER_H
