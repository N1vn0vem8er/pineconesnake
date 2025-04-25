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
    int rc = sqlite3_open(QString("%1/%2").arg(Settings::databasePath).arg(Settings::databaseName).toStdString().c_str(), &database);
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

QList<Note> ResourcesManager::getNotesForTitle(const QString title)
{
    QList<Note> notes;
    std::vector<std::vector<QString>> resoult;
    char* err;
    char* query;
    asprintf(&query, "SELECT * FROM notes WHERE title LIKE '%s%s%s' LIMIT 10;", "%", title.toStdString().c_str(), "%");
    if(sqlite3_exec(database, query, callback, &resoult, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    for(const auto& i : resoult)
    {
        notes.append(Note(i[0].toInt(), i[1], i[2], i[3], i[4]));
    }
    delete[] query;
    return notes;
}

int ResourcesManager::getLastId()
{
    return sqlite3_last_insert_rowid(database);
}

void ResourcesManager::addNote(const Note &note)
{
    std::vector<std::vector<QString>> resoult;
    char* err;
    char* query;
    asprintf(&query, "INSERT INTO notes (title, contents, created, modified) VALUES ('%s', '%s', '%s', '%s');", note.title.toStdString().c_str(), note.content.toStdString().c_str(), note.created.toStdString().c_str(),
             note.modified.toStdString().c_str());
    if(sqlite3_exec(database, query, callback, &resoult, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
}

void ResourcesManager::editNote(const Note &note)
{
    std::vector<std::vector<QString>> resoult;
    char* err;
    char* query;
    asprintf(&query, "UPDATE notes SET title='%s', contents = '%s', created = '%s', modified = '%s' WHERE id = %i;", note.title.toStdString().c_str(), note.content.toStdString().c_str(), note.created.toStdString().c_str(),
             note.modified.toStdString().c_str(), note.id);
    if(sqlite3_exec(database, query, callback, &resoult, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
}

void ResourcesManager::deleteNote(const Note &note)
{
    std::vector<std::vector<QString>> resoult;
    char* err;
    char* query;
    asprintf(&query, "DELETE FROM notes WHERE id = %i;", note.id);
    if(sqlite3_exec(database, query, callback, &resoult, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
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

ResourcesManager* ResourcesManager::instancePtr = nullptr;
std::mutex ResourcesManager::mutex;
