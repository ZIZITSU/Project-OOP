#pragma once
#include<vector>
#include<string>
#include "Song.hpp"
#include "SongLibrary.hpp"
using namespace std;

class BasePlaylist{
    public:
        virtual void listAllSongs(const SongLibrary& library) const = 0;
        virtual void addSong(const int songID) = 0;
        virtual void removeSong(const int songID) = 0;
        ~BasePlaylist(){}
};

class Playlist: public BasePlaylist{
    private:
        string playlistName_;
        vector<int> songIDs_;
        int playlistID_;
        static int currentPlaylistID;
    public:
        Playlist();
        Playlist(string name);
        ~Playlist();

        string getPlaylistName() const{return playlistName_;}
        int getPlaylistID() const{return playlistID_;}
        void setPlaylistName(string playlistName){playlistName_ = playlistName;}

        void addSong(const int songID) override;
        void removeSong(const int songID) override;
        void moveSong(const int moveFromIndex, const int moveToIndex);

        int getTotalDuration(const SongLibrary& library) const;
        const Song* getSong(int index, const SongLibrary& library) const;
        virtual void listAllSongs(const SongLibrary& library) const override;

        Playlist operator + (const Playlist& playlist) const;
        Playlist operator + (const Song& song) const;
        Playlist& operator += (const Playlist& playlist);
        Playlist& operator += (const Song& song);
};