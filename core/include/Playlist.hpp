#pragma once
#include <string>
#include <vector>
#include "Song.hpp"
#include "SongLibrary.hpp"

using namespace std;

class Playlist {
private:
    string name_;
    vector<int> songIds_;
    SongLibrary* library_;

public:
    Playlist(const string& name, SongLibrary* library);

    const string& name() const;
    const vector<int>& songIds() const;

    bool addSong(int songId);
    bool removeSong(int songId);
    vector<const Song*> getSongs() const;

    vector<const Song*> searchTitle(const string& keyword) const;
    vector<const Song*> searchArtist(const string& keyword) const;
};