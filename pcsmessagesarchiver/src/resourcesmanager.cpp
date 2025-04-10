#include "resourcesmanager.h"
#include "settings.h"
#include <sqlite3.h>

ResourcesManager::ResourcesManager()
{
    assert(!Settings::databasePath.isEmpty() && "databasePath must be loaded before used");
    assert(!Settings::databaseName.isEmpty() && "databaseName must be loaded before used");
    int rc = sqlite3_open(QString("%1/%2").arg(Settings::databasePath).arg(Settings::databaseName).toStdString().c_str(), &database);
    if(rc == SQLITE_OK)
    {
        char* err;
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS messages (id INTEGER PRIMARY KEY AUTOINCREMENT, title VARCHAR(32), contents VARCHAR(1000), sender VARCHAR(32), reciver VARCHAR(32), created varchar(32), modified varchar(32));", nullptr, nullptr, &err);
        if(rc != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS contacts (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(128));", nullptr, nullptr, &err);
        if(rc != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
    }
}

void ResourcesManager::close()
{
    sqlite3_close(database);
}

ResourcesManager::~ResourcesManager()
{
    close();
}

int ResourcesManager::callback(void *data, int argc, char **argv, char **azColName)
{
    std::vector<std::vector<QString>> *results = reinterpret_cast<std::vector<std::vector<QString>>*>(data);
    std::vector<QString> row;
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "NULL");
    }
    results->push_back(row);
    return 0;
}

ResourcesManager* ResourcesManager::getInstance()
{
    if(instancePtr == nullptr)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if(instancePtr == nullptr)
        {
            instancePtr = new ResourcesManager();
        }
    }
    return instancePtr;
}

void ResourcesManager::saveMessage(Globals::Message message)
{
    char* err;
    char* query;
    asprintf(&query, "INSERT INTO MESSAGES (title, contents, sender, reciver, created, modified) VALUES ('%s', '%s', '%s', '%s', '%s', '%s');", message.title.toStdString().c_str(), message.contents.toStdString().c_str(),
             message.from.toStdString().c_str(), message.to.toStdString().c_str(), message.created.toStdString().c_str(), message.modified.toStdString().c_str());
    sqlite3_exec(database, query, callback, nullptr, &err);
    if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    saveContactInNotExists(message.from);
    saveContactInNotExists(message.to);
}

QStringList ResourcesManager::loadMessagesTitles()
{
    char* err;
    std::vector<std::vector<QString>> messagesTitles;
    if(sqlite3_exec(database, "SELECT title FROM messages;", callback, &messagesTitles, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    QStringList ret;
    if(!messagesTitles.empty())
    {
        for(const auto& i : messagesTitles[0])
        {
            ret << i;
        }
    }
    return ret;
}

void ResourcesManager::closeConnection()
{
    close();
}

Globals::Message ResourcesManager::getMessageForTitle(const QString &title)
{
    std::vector<std::vector<QString>> msgs;
    char* query;
    char* err;
    asprintf(&query, "SELECT * FROM messages WHERE title = '%s';", title.toStdString().c_str());
    if(sqlite3_exec(database, query, callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    if(msgs.size() == 0 && msgs[0].size()<6)
    {
        throw std::exception();
    }
    delete[] query;
    Globals::Message msg(msgs[0][0].toInt(), msgs[0][1], msgs[0][2], msgs[0][3], msgs[0][4], msgs[0][5], msgs[0][6]);
    return msg;
}

Globals::Message ResourcesManager::getMessageById(int id)
{
    std::vector<std::vector<QString>> msgs;
    char* query;
    char* err;
    asprintf(&query, "SELECT * FROM messages WHERE id = %s;", std::to_string(id).c_str());
    if(sqlite3_exec(database, query, callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    if(msgs.size() == 0 && msgs[0].size()<6)
    {
        throw std::exception();
    }
    delete[] query;
    Globals::Message msg(msgs[0][0].toInt(), msgs[0][1], msgs[0][2], msgs[0][3], msgs[0][4], msgs[0][5], msgs[0][6]);
    return msg;
}

QList<Globals::Message> ResourcesManager::getRecentMessages()
{
    QList<Globals::Message> recent;
    std::vector<std::vector<QString>> msgs;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM messages ORDER BY datetime(modified) DESC LIMIT 30;", callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : msgs)
    {
        recent.append(Globals::Message(i[0].toInt(), i[1], i[2], i[3], i[4], i[5], i[6]));
    }
    return recent;
}

QList<Globals::Message> ResourcesManager::getAllMessages()
{
    QList<Globals::Message> all;
    std::vector<std::vector<QString>> msgs;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM messages;", callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : msgs)
    {
        all.append(Globals::Message(i[0].toInt(), i[1], i[2], i[3], i[4], i[5], i[6]));
    }
    return all;
}

void ResourcesManager::updateMessage(Globals::Message msg)
{
    std::vector<std::vector<QString>> msgs;
    char* query;
    asprintf(&query, "UPDATE messages SET title='%s', contents = '%s', sender = '%s', reciver = '%s', created = '%s', modified = '%s' WHERE id = %i;", msg.title.toStdString().c_str(),
             msg.contents.toStdString().c_str(), msg.from.toStdString().c_str(), msg.to.toStdString().c_str(), msg.created.toStdString().c_str(), msg.modified.toStdString().c_str(), msg.id);
    char* err;
    if(sqlite3_exec(database, query, callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
    saveContactInNotExists(msg.from);
    saveContactInNotExists(msg.to);
}

QList<Globals::Contact> ResourcesManager::getAllContacts()
{
    QList<Globals::Contact> all;
    std::vector<std::vector<QString>> msgs;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM contacts;", callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : msgs)
    {
        all.append(Globals::Contact(i[0].toInt(), i[1]));
    }
    return all;
}

void ResourcesManager::saveContactInNotExists(const QString &name)
{
    for(const auto& i : getAllContacts())
    {
        if(i.name == name)
        {
            return;
        }
    }
    char* query;
    asprintf(&query, "INSERT INTO contacts (name) VALUES ('%s');", name.toStdString().c_str());
    char* err;
    if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
}

void ResourcesManager::deleteMessageById(int id)
{
    std::vector<std::vector<QString>> msgs;
    char* query;
    asprintf(&query, "DELETE FROM messages WHERE id = %i;", id);
    char* err;
    if(sqlite3_exec(database, query, callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
}

void ResourcesManager::deleteContactById(int id)
{
    std::vector<std::vector<QString>> msgs;
    char* query;
    char* err;
    asprintf(&query, "DELETE FROM contacts WHERE id = %i;", id);
    if(sqlite3_exec(database, query, callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
}

ResourcesManager* ResourcesManager::instancePtr = nullptr;
std::mutex ResourcesManager::mutex;
