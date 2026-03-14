#pragma once
#include <vector>
#include <string>
#include "SongLibrary.hpp"
#include "Playlist.hpp"

using namespace std;

class Recommender {
private:
    const SongLibrary* library_;

public:
    Recommender(const SongLibrary* library);

    vector<const Song*> recommendByGenre(const string& genre, int maxResults = 10) const;
    vector<const Song*> recommendByMood(const string& mood, int maxResults = 10) const;
    vector<const Song*> recommendByArtist(const string& artist, int maxResults = 10) const;

    vector<const Song*> recommendForPlaylist(const Playlist& playlist, int maxResults = 10) const;
};
