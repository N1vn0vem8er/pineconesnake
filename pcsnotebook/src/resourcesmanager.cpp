#include "resourcesmanager.h"
#include "qdir.h"
#include "qobject.h"
#include "settings.h"

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
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS notes (id INTEGER PRIMARY KEY AUTOINCREMENT, title VARCHAR(32), contents VARCHAR(100000), created varchar(32), modified varchar(32));", nullptr, nullptr, &err);
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

QList<Note> ResourcesManager::getAllNotes()
{
    QList<Note> notes;
    std::vector<std::vector<QString>> resoult;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM notes;", callback, &resoult, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : resoult)
    {
        notes.append(Note(i[0].toInt(), i[1], i[2], i[3], i[4]));
    }
    return notes;
}

QList<Note> ResourcesManager::getNotesForTitle(const QString& title)
{
    QList<Note> notes;
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT * FROM notes WHERE title LIKE ? LIMIT 10;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        const QString searchPattern = QString("%%1%").arg(title);
        sqlite3_bind_text(stmt, 1, searchPattern.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            QString col1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            QString col2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            QString col3 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            QString col4 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

            notes.append(Note(id, col1, col2, col3, col4));
        }
    }
    sqlite3_finalize(stmt);
    return notes;
}

int ResourcesManager::getLastId()
{
    return sqlite3_last_insert_rowid(database);
}

void ResourcesManager::addNote(const Note &note)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "INSERT INTO notes (title, contents, created, modified) VALUES (?, ?, ?, ?);";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, note.title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, note.content.toStdString().c_str(), -1, SQLITE_TRANSIENT);;
        sqlite3_bind_text(stmt, 3, note.created.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, note.modified.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}

void ResourcesManager::editNote(const Note &note)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "UPDATE notes SET title=?, contents = ?, created = ?, modified = ? WHERE id = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, note.title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, note.content.toStdString().c_str(), -1, SQLITE_TRANSIENT);;
        sqlite3_bind_text(stmt, 3, note.created.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, note.modified.toStdString().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, note.id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
}

void ResourcesManager::deleteNote(const Note &note)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "DELETE FROM notes WHERE id = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, note.id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << QString("Failed to execute statement: %1").arg(sqlite3_errmsg(database));
        }
    }
    sqlite3_finalize(stmt);
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
