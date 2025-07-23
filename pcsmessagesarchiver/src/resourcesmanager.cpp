#include "resourcesmanager.h"
#include "settings.h"
#include <sqlite3.h>

ResourcesManager::ResourcesManager()
{
    assert(!Settings::databasePath.isEmpty() && "databasePath must be loaded before used");
    assert(!Settings::databaseName.isEmpty() && "databaseName must be loaded before used");
    int rc = sqlite3_open(QString("%1/%2").arg(Settings::databasePath, Settings::databaseName).toStdString().c_str(), &database);
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
        sqlite3_free(err);
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
    Q_UNUSED(azColName);
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
    static ResourcesManager instancePtr;
    return &instancePtr;
}

int ResourcesManager::saveMessage(Globals::Message message)
{
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO MESSAGES (title, contents, sender, reciver, created, modified) VALUES (?, ?, ?, ?, ?, ?);";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, message.title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, message.contents.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, message.from.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, message.to.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, message.created.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, message.modified.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
    saveContactInNotExists(message.from);
    saveContactInNotExists(message.to);
    return sqlite3_last_insert_rowid(database);
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
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM messages WHERE title = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            return Globals::Message(sqlite3_column_int(stmt, 0), QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                                    QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))), QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),
                                    QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))), QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))),
                                    QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
        }
    }
    return Globals::Message();
}

Globals::Message ResourcesManager::getMessageById(int id)
{
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM messages WHERE id = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            return Globals::Message(sqlite3_column_int(stmt, 0), QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                                    QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))), QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),
                                    QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))), QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))),
                                    QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
        }
    }
    return Globals::Message();
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

void ResourcesManager::updateMessage(Globals::Message message)
{
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE messages SET title = ?, contents = ?, sender = ?, reciver = ?, created = ?, modified = ? WHERE id = ?;";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, message.title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, message.contents.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, message.from.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, message.to.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, message.created.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, message.modified.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 7, message.id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
    saveContactInNotExists(message.from);
    saveContactInNotExists(message.to);
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
    const auto contacts = getAllContacts();
    for(const auto& i : contacts)
    {
        if(i.name == name)
        {
            return;
        }
    }
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO contacts (name) VALUES (?);";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, name.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}

void ResourcesManager::deleteMessageById(int id)
{
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM messages WHERE id = ?;";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}

void ResourcesManager::deleteContactById(int id)
{
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM contacts WHERE id = ?;";
    if(sqlite3_prepare(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}
