#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Song.hpp"

using namespace std;

class SongLibrary {
private:
    unordered_map<int, Song> songs_;

public:
    bool addSong(const Song& s);

    Song* getSong(int id);
    const Song* getSong(int id) const;
    
    vector<const Song*> searchTitle(const string& keyword) const;
    vector<const Song*> searchArtist(const string& keyword) const;


    vector<const Song*> getAllSongs() const;
};
