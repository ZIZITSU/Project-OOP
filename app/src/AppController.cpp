#include "AppController.hpp"
#include <QDebug>

using namespace std;

AppController::AppController(QObject *parent): QObject(parent), m_library(make_unique<SongLibrary>()) {}

AppController::~AppController() {}

bool AppController::loadSongsFromCSV(const QString& path) {
    qDebug() << "Loading songs from:" << path;
    
    bool success = m_library->loadFromCSV(path);
    
    if (success) {
        int count = m_library->getSongCount();
        qDebug() << "Successfully loaded" << count << "songs";
        emit songsLoaded();
        emit songCountChanged(count);
    } 
    else {
        qDebug() << "Failed to load songs from CSV";
    }
    
    return success;
}

int AppController::getSongCount() const {
    return m_library->getSongCount();
}

vector<shared_ptr<Song>> AppController::getAllSongs() const {
    return m_library->getAllSongs();
}

vector<shared_ptr<Song>> AppController::searchSongs(const QString& query) const {
    if (query.isEmpty()) {
        return getAllSongs();
    }
    
    // Searching in both title and artist
    auto titleResults = m_library->searchByTitle(query);
    auto artistResults = m_library->searchByArtist(query);
    
    // Combining results 
    vector<shared_ptr<Song>> combined = titleResults;
    
    for (const auto& song : artistResults) {
        bool exists = false;
        for (const auto& existing : combined) {
            if (existing->getId() == song->getId()) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            combined.push_back(song);
        }
    }
    
    return combined;
}

vector<shared_ptr<Song>> AppController::filterByMood(const QString& mood) const {
    return m_library->filterByMood(mood);
}

shared_ptr<Song> AppController::getSongById(int id) const {
    return m_library->getSongById(id);
}
