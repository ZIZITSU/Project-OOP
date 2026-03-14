#include "Song.hpp"
#include <utility>

Song::Song(int id, string t, string ar, string al, int dur, string g, vector<string> moods)
    : id_(id), title_(move(t)), artist_(move(ar)), album_(move(al)),
      genre_(move(g)), durationSec_(dur), moods_(move(moods)), rating_(0), playCount_(0) {}

int Song::rating() const {
    return rating_;
}

int Song::playCount() const {
    return playCount_;
}

int Song::id() const {
    return id_;
}

const string& Song::album() const {
    return album_;
}

int Song::duration() const {
    return durationSec_;
}

const string& Song::genre() const {
    return genre_;
}

const vector<string>& Song::moods() const {
    return moods_;
}

const string& Song::title() const {
    return title_;
}

const string& Song::artist() const {
    return artist_;
}

void Song::rate(int r) {
    if (r >= 1 && r <= 5)
        rating_ = r;
}

void Song::incrementPlay() {
    ++playCount_;
}