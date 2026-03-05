#pragma once
#include<vector>
#include<string>
#include "Song.hpp"
#include "SongLibrary.hpp"
using namespace std;

class Playlist{
    private:
        string playlistName_;
        vector<int> songIDs_;
    public:
        Playlist();
        Playlist(string name);
        ~Playlist();
        void addSong(const int songID);
        void removeSong(const int songID);
        void moveSong(const int moveFromIndex, const int moveToIndex);
        int getTotalDuration(const SongLibrary& library) const;
        const Song* getSong(int index, const SongLibrary& library) const;
        virtual void listAllSongs(const SongLibrary& library) const;
        Playlist operator + (const Playlist& playlist) const;
        Playlist operator + (const Song& song) const;
        Playlist& operator += (const Playlist& playlist);
        Playlist& operator += (const Song& song);
};