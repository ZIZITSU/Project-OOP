#pragma once
#include <string>
#include <vector>

using namespace std;

class Song {
    int id_;
    string title_, artist_, album_, genre_;
    int durationSec_;
    vector<string> moods_;
    int rating_{0};
    int playCount_{0};

public:
    Song(int id, string t, string ar, string al, int dur,string g, vector<string> moods): id_(id), title_(move(t)), 
    artist_(move(ar)), album_(move(al)),genre_(move(g)), durationSec_(dur), moods_(move(moods)) {}


    int rating() const { 
        return rating_; 
    }

    int playCount() const { 
        return playCount_; 
    }

    int id() const { 
        return id_; 
    }

    const std::string& album() const { return album_; }
    int duration() const { return durationSec_; }      // OR duration_ (use your real variable name)
    const std::string& genre() const { return genre_; }
    const std::vector<std::string>& moods() const { return moods_; }


    const string& title() const { 
        return title_; 
    }

    const string& artist() const { 
        return artist_; 
    }

    void rate(int r) { 
        if (r >= 1 && r <= 5){
            rating_ = r;
        } 
    }

    void incrementPlay() { 
        ++playCount_; 
    }
};
