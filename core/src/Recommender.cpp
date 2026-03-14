#include "Recommender.hpp"
#include <algorithm>

using namespace std;

Recommender::Recommender(const SongLibrary* library)
    : library_(library) {}

vector<const Song*> Recommender::recommendByGenre(const string& genre, int maxResults) const {
    vector<const Song*> result;
    string g = genre;
    transform(g.begin(), g.end(), g.begin(), ::tolower);

    for (const Song* s : library_->getAllSongs()) {
        string songGenre = s->genre();
        transform(songGenre.begin(), songGenre.end(), songGenre.begin(), ::tolower);
        if (songGenre == g) result.push_back(s);
        if ((int)result.size() >= maxResults) break;
    }
    return result;
}

vector<const Song*> Recommender::recommendByMood(const string& mood, int maxResults) const {
    vector<const Song*> result;
    string m = mood;
    transform(m.begin(), m.end(), m.begin(), ::tolower);

    for (const Song* s : library_->getAllSongs()) {
        for (const string& sm : s->moods()) {
            string songMood = sm;
            transform(songMood.begin(), songMood.end(), songMood.begin(), ::tolower);
            if (songMood == m) {
                result.push_back(s);
                break;
            }
        }
        if ((int)result.size() >= maxResults) break;
    }
    return result;
}

vector<const Song*> Recommender::recommendByArtist(const string& artist, int maxResults) const {
    vector<const Song*> result;
    string a = artist;
    transform(a.begin(), a.end(), a.begin(), ::tolower);

    for (const Song* s : library_->getAllSongs()) {
        string songArtist = s->artist();
        transform(songArtist.begin(), songArtist.end(), songArtist.begin(), ::tolower);
        if (songArtist == a) result.push_back(s);
        if ((int)result.size() >= maxResults) break;
    }
    return result;
}

vector<const Song*> Recommender::recommendForPlaylist(const Playlist& playlist, int maxResults) const {
    vector<const Song*> result;
    unordered_map<string, int> genreCount;

    for (const Song* s : playlist.getSongs()) {
        string g = s->genre();
        transform(g.begin(), g.end(), g.begin(), ::tolower);
        genreCount[g]++;
    }

    vector<pair<string,int>> sortedGenres(genreCount.begin(), genreCount.end());
    sort(sortedGenres.begin(), sortedGenres.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    for (const auto& [genre, _] : sortedGenres) {
        vector<const Song*> candidates = recommendByGenre(genre, maxResults - result.size());
        for (const Song* s : candidates) {
            if (find(result.begin(), result.end(), s) == result.end() &&
                find_if(playlist.getSongs().begin(), playlist.getSongs().end(),
                        [s](const Song* ps){ return ps->id() == s->id(); }) == playlist.getSongs().end()) {
                result.push_back(s);
            }
            if ((int)result.size() >= maxResults) break;
        }
        if ((int)result.size() >= maxResults) break;
    }

    return result;
}
