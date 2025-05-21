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
    int rc = sqlite3_open(QString("%1/%2").arg(Settings::databasePath, Settings::databaseName).toStdString().c_str(), &database);
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
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS playlists (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(128));", nullptr, nullptr, &err);
        if(rc != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
        rc = sqlite3_exec(database, "CREATE TABLE IF NOT EXISTS playlists_tracks (playlist_id INTEGER, track_id INTEGER, FOREIGN KEY (playlist_id) REFERENCES playlists(id), FOREIGN KEY (track_id) REFERENCES tracks(id), PRIMARY KEY (playlist_id, track_id));", nullptr, nullptr, &err);
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
    if(sqlite3_exec(database, "SELECT * FROM tracks WHERE favorite = true;", callback, &ret, &err) != SQLITE_OK)
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

QList<QString> ResourcesManager::getAllAlbums()
{
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT DISTINCT album FROM tracks;", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    QStringList names;
    std::for_each(ret.begin(), ret.end(), [&names](std::vector<QString> i){names.append(i[0]);});
    return names;
}

QList<QString> ResourcesManager::getAllArtists()
{
    std::vector<std::vector<QString>> ret;
    char* err;
    if(sqlite3_exec(database, "SELECT DISTINCT artist FROM tracks;", callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    QStringList names;
    std::for_each(ret.begin(), ret.end(), [&names](std::vector<QString> i){names.append(i[0]);});
    return names;
}

QList<Track> ResourcesManager::getAllTrackInAlbum(const QString &album)
{
    std::vector<std::vector<QString>> ret;
    char* err;
    char* query;
    asprintf(&query, "SELECT * FROM tracks WHERE album = \'%s\';", album.toStdString().c_str());
    if(sqlite3_exec(database, query, callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    QList<Track> tracks;
    for(const auto& i : ret)
    {
        tracks.append(Track(i[0].toInt(), i[2], i[1], i[3], i[4], i[5].toInt(), i[6].toInt(), i[7].toInt(), i[8].toInt()));
    }
    return tracks;
}

QList<Track> ResourcesManager::getAllTracksForArtist(const QString &artist)
{
    std::vector<std::vector<QString>> ret;
    char* err;
    char* query;
    asprintf(&query, "SELECT * FROM tracks WHERE artist = \'%s\';", artist.toStdString().c_str());
    if(sqlite3_exec(database, query, callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    QList<Track> tracks;
    for(const auto& i : ret)
    {
        tracks.append(Track(i[0].toInt(), i[2], i[1], i[3], i[4], i[5].toInt(), i[6].toInt(), i[7].toInt(), i[8].toInt()));
    }
    return tracks;
}

void ResourcesManager::savePlaylist(const Playlist &playlist)
{
    if(getPlaylistByName(playlist.name).id != -1)
        throw std::invalid_argument("Playlist with that name already exists");
    char* err;
    char* query;
    asprintf(&query, "INSERT INTO playlists (name) VALUES (\"%s\");", playlist.name.toStdString().c_str());
    if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    const int id = sqlite3_last_insert_rowid(database);
    for(const auto& i : playlist.tracks)
    {
        asprintf(&query, "INSERT INTO playlists_tracks (playlist_id, track_id) VALUES (%i, %i);", id, i.id);
        if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
        {
            printf("%s", err);
            sqlite3_free(err);
            throw std::exception();
        }
        delete[] query;
    }
}

void ResourcesManager::modifyPlaylist(const Playlist &playlist)
{
    char* err;
    char* query;
    Playlist old = getPlaylistByName(playlist.name);
    for(const auto& i : std::as_const(old.tracks))
    {
        if(std::find_if(playlist.tracks.begin(), playlist.tracks.end(), [&i](const Track& t){return t.id == i.id;}) == playlist.tracks.end())
        {
            asprintf(&query, "DELETE FROM playlists_tracks WHERE playlist_id = %i AND track_id = %i;", playlist.id, i.id);
            if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
            {
                printf("%s", err);
                sqlite3_free(err);
            }
            delete[] query;
        }
    }

    for(const auto& i : playlist.tracks)
    {
        asprintf(&query, "INSERT INTO playlists_tracks (playlist_id, track_id) VALUES (%i, %i);", playlist.id, i.id);
        sqlite3_exec(database, query, callback, nullptr, nullptr);
        delete[] query;
    }
}

Playlist ResourcesManager::getPlaylistById(const int id)
{
    std::vector<std::vector<QString>> ret;
    char* err;
    char* query;
    asprintf(&query, "SELECT * FROM playlists WHERE id = %i;", id);
    if(sqlite3_exec(database, query, callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    if(ret.empty()) return Playlist();
    const int pId = ret[0][0].toInt();
    const QString name = ret[0][1];
    asprintf(&query, "SELECT track_id FROM playlists_tracks WHERE playlist_id = %i;", id);
    if(sqlite3_exec(database, query, callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    QList<Track> tracks;
    std::vector<std::vector<QString>> retLoc;
    for(const auto& i : ret)
    {
        tracks.append(getTrackById(i[0].toInt()));
    }
    return Playlist(pId, name, tracks);
}

Playlist ResourcesManager::getPlaylistByName(const QString &name)
{
    std::vector<std::vector<QString>> ret;
    char* err;
    char* query;
    asprintf(&query, "SELECT * FROM playlists WHERE name = \'%s\';", name.toStdString().c_str());
    if(sqlite3_exec(database, query, callback, &ret, nullptr) != SQLITE_OK)
    {
        delete[] query;
        return Playlist(-1, "", {});
    }
    delete[] query;
    if(ret.empty()) return Playlist(-1, "", {});
    const int pId = ret[0][0].toInt();
    ret.clear();
    asprintf(&query, "SELECT track_id FROM playlists_tracks WHERE playlist_id = %i;", pId);
    if(sqlite3_exec(database, query, callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    QList<Track> tracks;
    std::vector<std::vector<QString>> retLoc;
    for(const auto& i : ret)
    {
        tracks.append(getTrackById(i[0].toInt()));
    }
    return Playlist(pId, name, tracks);
}

QList<Playlist> ResourcesManager::getPlaylists()
{
    return {};
}

Track ResourcesManager::getTrackById(int id)
{
    std::vector<std::vector<QString>> ret;
    char* err;
    char* query;
    asprintf(&query, "SELECT * FROM tracks WHERE id = %i;", id);
    if(sqlite3_exec(database, query, callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    if(ret.empty()) return Track();
    return Track(ret[0][0].toInt(), ret[0][2], ret[0][1], ret[0][3], ret[0][4], ret[0][5].toInt(), ret[0][6].toInt(), ret[0][7].toInt(), ret[0][8].toInt());
}

QStringList ResourcesManager::getAllPlaylistNames()
{
    std::vector<std::vector<QString>> ret;
    char* err;
    char* query;
    asprintf(&query, "SELECT DISTINCT name FROM playlists;");
    if(sqlite3_exec(database, query, callback, &ret, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
        throw std::exception();
    }
    delete[] query;
    QStringList names;
    for(const auto& i : ret)
    {
        names.append(i[0]);
    }
    return names;
}

void ResourcesManager::deletePlaylist(int id)
{
    char* err;
    char* query;
    asprintf(&query, "DELETE FROM playlists_tracks WHERE playlist_id = %i;", id);
    if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
    asprintf(&query, "DELETE FROM playlists WHERE id = %i;", id);
    if(sqlite3_exec(database, query, callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    delete[] query;
}

void ResourcesManager::clearDatabase()
{
    char* err;
    if(sqlite3_exec(database, "DELETE FROM playlists_tracks;", callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    if(sqlite3_exec(database, "DELETE FROM playlists;", callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
    if(sqlite3_exec(database, "DELETE FROM tracks;", callback, nullptr, &err) != SQLITE_OK)
    {
        printf("%s", err);
        sqlite3_free(err);
    }
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
