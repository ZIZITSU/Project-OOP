#include "UserProfile.hpp"
#include <algorithm>

using namespace std;

UserProfile::UserProfile(const string& username, int userId, SongLibrary* library)
    : username_(username), userId_(userId), library_(library) {}

const string& UserProfile::username() const {
    return username_;
}

int UserProfile::userId() const {
    return userId_;
}

vector<Playlist>& UserProfile::playlists() {
    return playlists_;
}

const vector<Playlist>& UserProfile::playlists() const {
    return playlists_;
}

bool UserProfile::addPlaylist(const string& name) {
    auto it = find_if(playlists_.begin(), playlists_.end(),
                      [&name](const Playlist& p){ return p.name() == name; });
    if (it != playlists_.end()) return false;
    playlists_.emplace_back(name, library_);
    return true;
}

bool UserProfile::removePlaylist(const string& name) {
    auto it = find_if(playlists_.begin(), playlists_.end(),
                      [&name](const Playlist& p){ return p.name() == name; });
    if (it == playlists_.end()) return false;
    playlists_.erase(it);
    return true;
}

Playlist* UserProfile::getPlaylist(const string& name) {
    auto it = find_if(playlists_.begin(), playlists_.end(),
                      [&name](Playlist& p){ return p.name() == name; });
    if (it == playlists_.end()) return nullptr;
    return &(*it);
}

const Playlist* UserProfile::getPlaylist(const string& name) const {
    auto it = find_if(playlists_.begin(), playlists_.end(),
                      [&name](const Playlist& p){ return p.name() == name; });
    if (it == playlists_.end()) return nullptr;
    return &(*it);
}
