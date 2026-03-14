#pragma once
#include <string>
#include <vector>
#include "Playlist.hpp"
#include "SongLibrary.hpp"

using namespace std;

class UserProfile {
private:
    string username_;
    int userId_;
    vector<Playlist> playlists_;
    SongLibrary* library_;

public:
    UserProfile(const string& username, int userId, SongLibrary* library);

    const string& username() const;
    int userId() const;

    vector<Playlist>& playlists();
    const vector<Playlist>& playlists() const;

    bool addPlaylist(const string& name);
    bool removePlaylist(const string& name);
    Playlist* getPlaylist(const string& name);
    const Playlist* getPlaylist(const string& name) const;
};
