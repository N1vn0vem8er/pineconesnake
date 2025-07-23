#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "qobject.h"
#include <sqlite3.h>
#include <QDir>
#include "globals.h"
class ResourcesManager
{
public:
    static ResourcesManager *getInstance();
    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    void close() const;
    int saveMessage(Globals::Message message) const;
    QStringList loadMessagesTitles() const;
    void closeConnection() const;
    Globals::Message getMessageForTitle(const QString& title) const;
    Globals::Message getMessageById(int id) const;
    QList<Globals::Message> getRecentMessages() const;
    QList<Globals::Message> getAllMessages() const;
    void updateMessage(Globals::Message msg) const;
    QList<Globals::Contact> getAllContacts() const;
    void saveContactInNotExists(const QString& name) const;
    void deleteMessageById(int id) const;
    void deleteContactById(int id) const;
private:
    ResourcesManager();
    ~ResourcesManager();
    sqlite3* database = nullptr;
    std::vector<Globals::Message> messages;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCESMANAGER_H
