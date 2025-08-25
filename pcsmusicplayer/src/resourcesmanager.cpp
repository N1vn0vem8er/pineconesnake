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
    sqlite3_stmt* stmt;
    const char* sql = "INSERT OR IGNORE INTO tracks (path, title, artist, album, number, length, played, favorite) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        for(const auto& i : tracks)
        {
            sqlite3_bind_text(stmt, 1, i.path.toUtf8(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, i.title.toUtf8(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, i.artist.toUtf8(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, i.album.toUtf8(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 5, i.number);
            sqlite3_bind_int(stmt, 6, i.length);
            sqlite3_bind_int(stmt, 7, i.played);
            sqlite3_bind_int(stmt, 8, i.favorite);
            if(sqlite3_step(stmt) != SQLITE_DONE)
            {
                qDebug() << sqlite3_errmsg(database);
                sqlite3_finalize(stmt);
                return;
            }
            sqlite3_reset(stmt);
        }
        sqlite3_finalize(stmt);
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
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE tracks SET path = ?, title = ?, artist = ?, album = ?, number = ?, length = ?, played = ?, favorite = ? WHERE id = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, track.path.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, track.title.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, track.artist.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, track.album.toUtf8(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, track.number);
        sqlite3_bind_int(stmt, 6, track.length);
        sqlite3_bind_int(stmt, 7, track.played);
        sqlite3_bind_int(stmt, 8, track.favorite);
        sqlite3_bind_int(stmt, 9, track.id);
        if(sqlite3_step(stmt) != SQLITE_DONE)
        {
            qDebug() << sqlite3_errmsg(database);
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_finalize(stmt);
    }
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
    QList<Track> tracks;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM tracks WHERE artist = ?";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, artist.toUtf8(), -1, SQLITE_TRANSIENT);
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            tracks.append(Track(sqlite3_column_int(stmt, 0),
                                QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))),
                                QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                                QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),
                                QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))),
                                sqlite3_column_int(stmt, 5), sqlite3_column_int(stmt, 6),sqlite3_column_int(stmt, 7),sqlite3_column_int(stmt, 8)));
        }
        sqlite3_finalize(stmt);
    }
    return tracks;
}

void ResourcesManager::savePlaylist(const Playlist &playlist)
{
    if(getPlaylistByName(playlist.name).id != -1)
        throw std::invalid_argument("Playlist with that name already exists");

    sqlite3_stmt* stmt1;
    const char* sql1 = "INSERT OR IGNORE INTO playlists (name) VALUES (?);";
    if(sqlite3_prepare_v2(database, sql1, -1, &stmt1, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt1, 1, playlist.name.toUtf8(), -1, SQLITE_TRANSIENT);
        if(sqlite3_step(stmt1) != SQLITE_DONE)
        {
            qDebug() << sqlite3_errmsg(database);
            sqlite3_finalize(stmt1);
            return;
        }
        sqlite3_finalize(stmt1);
    }
    const int id = sqlite3_last_insert_rowid(database);
    sqlite3_stmt* stmt2;
    const char* sql2 = "INSERT OR IGNORE INTO playlists_tracks (playlist_id, track_id) VALUES (?, ?);";
    if(sqlite3_prepare_v2(database, sql2, -1, &stmt2, nullptr) == SQLITE_OK)
    {
        for(const auto& i : std::as_const(playlist.tracks))
        {
            sqlite3_bind_int(stmt2, 1, id);
            sqlite3_bind_int(stmt2, 2, i.id);
            if(sqlite3_step(stmt2) != SQLITE_DONE)
            {
                qDebug() << sqlite3_errmsg(database);
                sqlite3_finalize(stmt2);
                return;
            }
            sqlite3_reset(stmt2);
        }
        sqlite3_finalize(stmt2);
    }
}

void ResourcesManager::modifyPlaylist(const Playlist &playlist)
{
    sqlite3_stmt* delStmt;
    sqlite3_stmt* insStmt;
    const char* delQuery = "DELETE FROM playlists_tracks WHERE playlist_id = ? AND track_id = ?;";
    if(sqlite3_prepare_v2(database, delQuery, -1, &delStmt, nullptr) == SQLITE_OK)
    {
        Playlist old = getPlaylistByName(playlist.name);
        for(const auto& i : std::as_const(old.tracks))
        {
            if(std::find_if(playlist.tracks.begin(), playlist.tracks.end(), [&](const Track& t) { return t.id == i.id; }) == playlist.tracks.end())
            {
                sqlite3_bind_int(delStmt, 1, playlist.id);
                sqlite3_bind_int(delStmt, 2, i.id);
                if(sqlite3_step(delStmt) != SQLITE_DONE)
                {
                    qDebug() << sqlite3_errmsg(database);
                }
                sqlite3_reset(delStmt);
            }
        }
        sqlite3_finalize(delStmt);
    }
    const char* insQuery = "INSERT OR IGNORE INTO playlists_tracks (playlist_id, track_id) VALUES (?, ?);";
    if(sqlite3_prepare_v2(database, insQuery, -1, &insStmt, nullptr) == SQLITE_OK)
    {
        for(const auto& i : std::as_const(playlist.tracks))
        {
            sqlite3_bind_int(insStmt, 1, playlist.id);
            sqlite3_bind_int(insStmt, 2, i.id);
            if(sqlite3_step(insStmt) != SQLITE_DONE)
            {
                qDebug() << sqlite3_errmsg(database);
            }
            sqlite3_reset(insStmt);
        }
        sqlite3_finalize(insStmt);
    }
}

Playlist ResourcesManager::getPlaylistById(const int id)
{
    Playlist ret;
    ret.id = -1;
    int pid = -1;
    QString name;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM playlists WHERE id = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            pid = sqlite3_column_int(stmt, 0);
            name = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        }
        sqlite3_finalize(stmt);
    }
    if(pid != -1 && !name.isEmpty())
    {
        const char *sql = "SELECT track_id FROM playlists_tracks WHERE playlist_id = ?;";
        if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, pid);
            QList<Track> tracks;
            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                tracks.append(getTrackById(sqlite3_column_int(stmt, 0)));
            }
            sqlite3_finalize(stmt);
            ret = Playlist(pid, name, tracks);
        }
    }

    return ret;
}

Playlist ResourcesManager::getPlaylistByName(const QString &name)
{
    Playlist ret;
    ret.id = -1;
    int id = -1;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM playlists WHERE name = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, name.toUtf8(), -1, SQLITE_TRANSIENT);
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    if(id != -1)
    {
        const char *sql = "SELECT track_id FROM playlists_tracks WHERE playlist_id = ?;";
        if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
        {
            sqlite3_bind_int(stmt, 1, id);
            QList<Track> tracks;
            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                tracks.append(getTrackById(sqlite3_column_int(stmt, 0)));
            }
            sqlite3_finalize(stmt);
            ret = Playlist(id, name, tracks);
        }
    }

    return ret;
}

QList<Playlist> ResourcesManager::getPlaylists()
{
    return {};
}

Track ResourcesManager::getTrackById(int id)
{
    Track ret;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM tracks WHERE id = ?;";
    if(sqlite3_prepare_v2(database, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            ret = Track(sqlite3_column_int(stmt, 0), QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))),
                         QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))),
                         QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),
                         QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))),
                        sqlite3_column_int(stmt, 5), sqlite3_column_int(stmt, 6),sqlite3_column_int(stmt, 7),sqlite3_column_int(stmt, 8));
        }
        sqlite3_finalize(stmt);
    }
    return ret;
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

void ResourcesManager::removeFromPlaylist(int playlistId, int trackId)
{
    char* err;
    char* query;
    asprintf(&query, "DELETE FROM playlists_tracks WHERE playlist_id = %i AND track_id = %i;", playlistId, trackId);
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
    static ResourcesManager instancePtr;
    return &instancePtr;
}
