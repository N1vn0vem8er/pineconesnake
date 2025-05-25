#include "resourcesmanager.h"
#include "settings.h"
#include <QDir>
#include <cstdio>
#include <exception>

ResourcesManager::ResourcesManager()
{
    assert(!Settings::databasePath.isEmpty() && "databasePath must be loaded before used");
    assert(!Settings::databaseName.isEmpty() && "databaseName must be loaded before used");
    if(!QDir(Settings::databasePath).exists())
    {
        QDir().mkpath(Settings::databasePath);
    }
    int rc = sqlite3_open(QString("%1/%2").arg(Settings::databasePath, Settings::databaseName).toStdString().c_str(), &database);
    if(rc == SQLITE_OK)
    {
        char* err;
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS events (id INTEGER PRIMARY KEY AUTOINCREMENT, title VARCHAR(128), content VARCHAR(1000), date TEXT, enabled INTEGER);", nullptr, nullptr, &err);
        if(rc != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS repeated (id INTEGER PRIMARY KEY AUTOINCREMENT, title VARCHAR(128), content VARCHAR(1000), everySeconds INTEGER, enabled INTEGER);", nullptr, nullptr, &err);
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

void ResourcesManager::saveEvent(const EventManager::Event &event)
{
    char* err;
    char* query;
    asprintf(&query, "INSERT INTO events (title, content, date, enabled) VALUES ('%s', '%s', '%s', %i);", event.title.toStdString().c_str(),
             event.content.toStdString().c_str(), event.date.toStdString().c_str(), event.enabled);
    if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
}

void ResourcesManager::saveRepeating(const EventManager::RepeatedEvent &event)
{
    char* err;
    char* query;
    asprintf(&query, "INSERT INTO repeated (title, content, repeat, date, enabled) VALUES ('%s', '%s', %i, %i);", event.title.toStdString().c_str(),
             event.content.toStdString().c_str(), event.everySeconds, event.enabled);
    if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
}

int ResourcesManager::callback(void *data, int argc, char **argv, char **azColName)
{

}

ResourcesManager::~ResourcesManager()
{
    close();
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

ResourcesManager* ResourcesManager::instancePtr = nullptr;
std::mutex ResourcesManager::mutex;
