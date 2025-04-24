#include "resourcesmanager.h"
#include "qobject.h"
#include "settings.h"

ResourcesManager::ResourcesManager()
{
    assert(!Settings::databasePath.isEmpty() && "databasePath must be loaded before used");
    assert(!Settings::databaseName.isEmpty() && "databaseName must be loaded before used");
    int rc = sqlite3_open(QString("%1/%2").arg(Settings::databasePath).arg(Settings::databaseName).toStdString().c_str(), &database);
    if(rc == SQLITE_OK)
    {
        char* err;
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
