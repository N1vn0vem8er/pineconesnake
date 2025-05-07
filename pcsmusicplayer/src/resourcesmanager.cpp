#include "resourcesmanager.h"

#include "settings.h"

#include <QDir>
#include <QString>

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
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS tracks (id INTEGER PRIMARY KEY AUTOINCREMENT, path VARCHAR(1000), title VARCHAR(128), artist VARCHAR(128), album varchar(128), number INTEGER, length INTEGER, played INTEGER, favorite favorite);", nullptr, nullptr, &err);
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

void ResourcesManager::saveTracks(QList<Track> tracks)
{
    for(auto& i : tracks)
    {
        char* err;
        char* query;
        asprintf(&query, "INSERT INTO tracks (path, title, artist, album, number, length, played, favorite) VALUES (\"%s\", \"%s\", \"%s\", \"%s\", %i, %i, %i, %b);", i.path.toStdString().c_str(),
                 i.title.replace("\"", "\\\"").replace("\'", "\\\'").toStdString().c_str(), i.artist.replace("\"", "\\\"").replace("\'", "\\\'").toStdString().c_str(),
                 i.album.replace("\"", "\\\"").replace("\'", "\\\'").toStdString().c_str(), i.number, i.length, i.played, i.favorite);
        if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
        delete[] query;
    }
}

QStringList ResourcesManager::getAllPaths()
{
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT path FROM tracks", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    QStringList paths;
    for(const auto& i : ret)
    {
        paths.append(i[0]);
    }
    return paths;
}

QList<Track> ResourcesManager::getAllTracks()
{
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM tracks", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    QList<Track> tracks;
    for(const auto& i : ret)
    {
        tracks.append(Track(i[0].toInt(), i[2], i[1], i[3], i[4], i[5].toInt(), i[6].toInt(), i[7].toInt(), i[8].toInt()));
    }
    return tracks;
}

void ResourcesManager::modifyTrack(Track track)
{
    char* err;
    char* query;
    asprintf(&query, "UPDATE tracks SET path = \"%s\", title = \"%s\", artist = \"%s\", album = \"%s\", number = %i, length = %i, played = %i, favorite = %b WHERE id = %i;", track.path.toStdString().c_str(),
             track.title.replace("\"", "\\\"").replace("\'", "\\\'").toStdString().c_str(), track.artist.replace("\"", "\\\"").replace("\'", "\\\'").toStdString().c_str(),
             track.album.replace("\"", "\\\"").replace("\'", "\\\'").toStdString().c_str(), track.number, track.length, track.played, track.favorite, track.id);
    if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
}

QList<Track> ResourcesManager::getAllFavoriteTracks()
{
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT * FROM tracks WHERE favorite = true", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    QList<Track> tracks;
    for(const auto& i : ret)
    {
        tracks.append(Track(i[0].toInt(), i[2], i[1], i[3], i[4], i[5].toInt(), i[6].toInt(), i[7].toInt(), i[8].toInt()));
    }
    return tracks;
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
