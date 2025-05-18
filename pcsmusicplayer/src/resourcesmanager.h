#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include "structs.h"
#include <mutex>
#include <sqlite3.h>
class ResourcesManager
{
public:
    static ResourcesManager *getInstance();
    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    void close();
    void saveTracks(QList<Track> tracks);
    QStringList getAllPaths();
    QList<Track> getAllTracks();
    void modifyTrack(Track track);
    QList<Track> getAllFavoriteTracks();
    QList<QString> getAllAlbums();
    QList<QString> getAllArtists();
    QList<Track> getAllTrackInAlbum(const QString& album);
    QList<Track> getAllTracksForArtist(const QString& artist);
    void savePlaylist(const Playlist& playlist);
    void modifyPlaylist(const Playlist& playlist);
    Playlist getPlaylistById(const int id);
    Playlist getPlaylistByName(const QString& name);
    QList<Playlist> getPlaylists();
    Track getTrackById(int id);
    QStringList getAllPlaylistNames();
    void deletePlaylist(int id);
    void clearDatabase();

private:
    ResourcesManager();
    ~ResourcesManager();
    sqlite3* database = nullptr;
    static std::mutex mutex;
    static ResourcesManager* instancePtr;
    static int callback(void* data, int argc, char** argv, char **azColName);
};

#endif // RESOURCESMANAGER_H
