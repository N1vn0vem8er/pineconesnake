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
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS events (id INTEGER PRIMARY KEY AUTOINCREMENT, title VARCHAR(128), content VARCHAR(1000), date TEXT, enabled INTEGER, type INTEGER);", nullptr, nullptr, &err);
        if(rc != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS repeated (id INTEGER PRIMARY KEY AUTOINCREMENT, title VARCHAR(128), content VARCHAR(1000), everySeconds INTEGER, enabled INTEGER, type INTEGER);", nullptr, nullptr, &err);
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
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO events (title, content, date, enabled, type) VALUES (?, ?, ?, ?, ?);";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, event.title.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, event.content.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, event.date.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, event.enabled);
        sqlite3_bind_int(stmt, 5, event.type);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << sqlite3_errmsg(database);
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        qDebug() << sqlite3_errmsg(database);
    }
}

void ResourcesManager::saveRepeating(const EventManager::RepeatedEvent &event)
{
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO repeated (title, content, everySeconds, enabled, type) VALUES (?, ?, ?, ?, ?);";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, event.title.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, event.content.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, event.everySeconds);
        sqlite3_bind_int(stmt, 4, event.enabled);
        sqlite3_bind_int(stmt, 5, event.type);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << sqlite3_errmsg(database);
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        qDebug() << sqlite3_errmsg(database);
    }
}

QList<EventManager::RepeatedEvent> ResourcesManager::getAllRepeating()
{
    QList<EventManager::RepeatedEvent> ret;
    std::vector<std::vector<QString>> r;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM repeated;", callback, &r, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : r)
    {
        ret.append(EventManager::RepeatedEvent(i[0].toInt(), i[1], i[2], i[3].toInt(), i[4].toInt(), i[5].toInt()));
    }
    return ret;
}

QList<EventManager::Event> ResourcesManager::getAllEvents()
{
    QList<EventManager::Event> ret;
    std::vector<std::vector<QString>> r;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM events;", callback, &r, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : r)
    {
        ret.append(EventManager::Event(i[0].toInt(), i[1], i[2], i[3], i[4].toInt(), i[5].toInt()));
    }
    return ret;
}

void ResourcesManager::deleteRepeating(const EventManager::RepeatedEvent &event)
{
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM repeated WHERE id = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, event.id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << sqlite3_errmsg(database);
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        qDebug() << sqlite3_errmsg(database);
    }
}

void ResourcesManager::deleteEvent(const EventManager::Event &event)
{
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM events WHERE id = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, event.id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << sqlite3_errmsg(database);
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        qDebug() << sqlite3_errmsg(database);
    }
}

void ResourcesManager::modifyEvent(const EventManager::Event &event)
{
    std::vector<std::vector<QString>> msgs;
    char* err;
    if(sqlite3_exec(database, QString("UPDATE events SET title = \'%1\', content = \'%2\', date = \'%3\', enabled = %4, type = %5 WHERE id = %6;")
                                   .arg(event.title, event.content, event.date, QString::number(event.enabled), QString::number(event.type), QString::number(event.id)).toStdString().c_str(), callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
}

void ResourcesManager::modifyRepeating(const EventManager::RepeatedEvent &event)
{
    std::vector<std::vector<QString>> msgs;
    char* err;
    if(sqlite3_exec(database, QString("UPDATE repeated SET title = \'%1\', content = \'%2\', everySeconds = %3, enabled = %4, type = %5 WHERE id = %6;")
                                   .arg(event.title, event.content, QString::number(event.everySeconds), QString::number(event.enabled), QString::number(event.type), QString::number(event.id)).toStdString().c_str(), callback, &msgs, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
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
