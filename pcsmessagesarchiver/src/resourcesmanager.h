#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "qobject.h"
#include <mutex>
#include <sqlite3.h>
#include <QDir>
#include "globals.h"
class ResourcesManager
{
public:
    static ResourcesManager *getInstance();
    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    void close();
    void saveMessage(Globals::Message message);
    QStringList loadMessagesTitles();
    void closeConnection();
    Globals::Message getMessageForTitle(const QString& title);
    Globals::Message getMessageById(int id);
    QList<Globals::Message> getRecentMessages();
    QList<Globals::Message> getAllMessages();
    void updateMessage(Globals::Message msg);
    QList<Globals::Contact> getAllContacts();
    void saveContactInNotExists(const QString& name);
    void deleteMessageById(int id);
    void deleteContactById(int id);
private:
    ResourcesManager();
    ~ResourcesManager();
    sqlite3* database = nullptr;
    static std::mutex mutex;
    static ResourcesManager* instancePtr;
    std::vector<Globals::Message> messages;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCESMANAGER_H
