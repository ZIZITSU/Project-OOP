#include "Playlist.hpp"
#include <algorithm>

using namespace std;

Playlist::Playlist(const string& name, SongLibrary* library)
    : name_(name), library_(library) {}

const string& Playlist::name() const {
    return name_;
}

const vector<int>& Playlist::songIds() const {
    return songIds_;
}

bool Playlist::addSong(int songId) {
    if (library_->getSong(songId) == nullptr)
        return false;
    if (find(songIds_.begin(), songIds_.end(), songId) != songIds_.end())
        return false;
    songIds_.push_back(songId);
    return true;
}

bool Playlist::removeSong(int songId) {
    auto it = find(songIds_.begin(), songIds_.end(), songId);
    if (it == songIds_.end())
        return false;
    songIds_.erase(it);
    return true;
}

vector<const Song*> Playlist::getSongs() const {
    vector<const Song*> result;
    for (int id : songIds_) {
        const Song* s = library_->getSong(id);
        if (s != nullptr)
            result.push_back(s);
    }
    return result;
}

vector<const Song*> Playlist::searchTitle(const string& keyword) const {
    vector<const Song*> result;
    string key = keyword;
    transform(key.begin(), key.end(), key.begin(), ::tolower);

    for (int id : songIds_) {
        const Song* s = library_->getSong(id);
        if (s) {
            string t = s->title();
            transform(t.begin(), t.end(), t.begin(), ::tolower);
            if (t.find(key) != string::npos)
                result.push_back(s);
        }
    }
    return result;
}

vector<const Song*> Playlist::searchArtist(const string& keyword) const {
    vector<const Song*> result;
    string key = keyword;
    transform(key.begin(), key.end(), key.begin(), ::tolower);

    for (int id : songIds_) {
        const Song* s = library_->getSong(id);
        if (s) {
            string a = s->artist();
            transform(a.begin(), a.end(), a.begin(), ::tolower);
            if (a.find(key) != string::npos)
                result.push_back(s);
        }
    }
    return result;
}