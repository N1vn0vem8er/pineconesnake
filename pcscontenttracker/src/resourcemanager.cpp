#include "resourcemanager.h"
#include "qdebug.h"
#include "qdir.h"
#include "settings.h"
#include <QList>
#include <QString>
#include <cstdio>
#include <exception>

ResourceManager::ResourceManager()
{
    if(!QDir(Settings::databasePath).exists())
    {
        QDir().mkpath(Settings::databasePath);
    }
    int rc = sqlite3_open(QString("%1/%2").arg(Settings::databasePath, Settings::databaseName).toStdString().c_str(), &database);
    if(rc == SQLITE_OK)
    {
        char* err;
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS items (id INTEGER PRIMARY KEY AUTOINCREMENT, title VARCHAR(32), description VARCHAR(10000), status INTEGER, image VRACHAR(32), link VARCHAR(256), chapter INTEGER, chapters INTEGER, FOREIGN KEY (status) REFERENCES status(id));", nullptr, nullptr, &err);
        if(rc != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS status (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(128));", nullptr, nullptr, &err);
        if(rc != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
        rc = sqlite3_exec(database, "INSERT INTO status (id, name) VALUES (0, 'Planned');INSERT INTO status (id, name) VALUES (1, 'Current');INSERT INTO status (id, name) VALUES (2, 'Finished');", nullptr, nullptr, &err);
        if(rc != 19 && rc != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
    }
}

ResourceManager::~ResourceManager()
{
    close();
}

ResourceManager *ResourceManager::getInstance()
{
    static ResourceManager instancePtr;
    return &instancePtr;
}

void ResourceManager::close()
{
    sqlite3_close(database);
}

QList<Item> ResourceManager::getAllItems()
{
    QList<Item> items;
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM items;", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : ret)
    {
        items.push_back(Item(i[0].toInt(), i[1], i[2], i[3].toInt(), i[4], i[5], i[6].toInt(), i[7].toInt()));
    }
    return items;
}

QList<Status> ResourceManager::getAllStates()
{
    QList<Status> states;
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM status;", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : ret)
    {
        states.push_back(Status(i[0].toInt(), i[1]));
    }
    return states;
}

void ResourceManager::addItem(const Item &item)
{
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO items (title, description, status, image, link, chapters, chapter) VALUES (?, ?, ?, ?, ?, ?, 0);";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, item.title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, item.description.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, item.status);
        sqlite3_bind_text(stmt, 4, item.image.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, item.link.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 6, item.chapters);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}

void ResourceManager::deleteItem(const Item &item)
{
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM items WHERE id = ?;";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, item.id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}

void ResourceManager::editItem(const Item &item)
{
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE items SET title = ?, description = ?, status = ?, image = ?, link = ?, chapter = ?, chapters = ? WHERE id = ?;";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, item.title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, item.description.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, item.status);
        sqlite3_bind_text(stmt, 4, item.image.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, item.link.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 6, item.chapter);
        sqlite3_bind_int(stmt, 7, item.chapters);
        sqlite3_bind_int(stmt, 8, item.id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}

QList<Item> ResourceManager::getAllFinished()
{
    QList<Item> items;
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM items i JOIN status s ON i.status = s.id WHERE s.name = 'Finished';", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : ret)
    {
        items.push_back(Item(i[0].toInt(), i[1], i[2], i[3].toInt(), i[4], i[5], i[6].toInt(), i[7].toInt()));
    }
    return items;
}

QList<Item> ResourceManager::getAllCurrnet()
{
    QList<Item> items;
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM items i JOIN status s ON i.status = s.id WHERE s.name = 'Current';", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : ret)
    {
        items.push_back(Item(i[0].toInt(), i[1], i[2], i[3].toInt(), i[4], i[5], i[6].toInt(), i[7].toInt()));
    }
    return items;
}

QList<Item> ResourceManager::getAllPlanned()
{
    QList<Item> items;
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM items i JOIN status s ON i.status = s.id WHERE s.name = 'Planned';", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : ret)
    {
        items.push_back(Item(i[0].toInt(), i[1], i[2], i[3].toInt(), i[4], i[5], i[6].toInt(), i[7].toInt()));
    }
    return items;
}

void ResourceManager::setStatus(int itemId, int statusId)
{
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE items SET status = ? WHERE id = ?;";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, statusId);
        sqlite3_bind_int(stmt, 2, itemId);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}

void ResourceManager::setChapter(int id, int val)
{
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE items SET chapter = ? WHERE id = ?;";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, val);
        sqlite3_bind_int(stmt, 2, id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);

}

QList<Item> ResourceManager::get10ItemsByTitle(const QString &title)
{
    QList<Item> items;
    std::vector<std::vector<QString>> ret;
    char* err;
    char* query;
    asprintf(&query, "SELECT * FROM items WHERE title LIKE '%s%s%s' LIMIT 10;", "%", title.toStdString().c_str(), "%");
    if(sqlite3_exec(database, query, callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : ret)
    {
        items.push_back(Item(i[0].toInt(), i[1], i[2], i[3].toInt(), i[4], i[5], i[6].toInt(), i[7].toInt()));
    }
    delete[] query;
    return items;
}

int ResourceManager::callback(void *data, int argc, char **argv, char **azColName)
{
    Q_UNUSED(azColName);
    std::vector<std::vector<QString>> *results = reinterpret_cast<std::vector<std::vector<QString>>*>(data);
    std::vector<QString> row;
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "NULL");
    }
    results->push_back(row);
    return 0;
}
