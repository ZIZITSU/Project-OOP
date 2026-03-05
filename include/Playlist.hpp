#pragma once
#include<vector>
#include<string>
#include "Song.hpp"
#include "SongLibrary.hpp"
using namespace std;

class BasePlaylist{
    public:
        virtual void listAllPlaylistSongs() const = 0;
        virtual void addSong(const Song& song) = 0;
        virtual void removeSong(const Song& song) = 0;
        ~BasePlaylist(){}
};

class Playlist: public BasePlaylist{
    private:
        string playlistName_;
        vector<Song*> songs_;
        int playlistID_;
        static int currentPlaylistID;
        SongLibrary* library_;
    public:
        Playlist(const SongLibrary& library);
        Playlist(const SongLibrary& library, string name);
        ~Playlist();

        string getPlaylistName() const{return playlistName_;}
        int getPlaylistID() const{return playlistID_;}
        const Song* getSongAtIndex(int index) const{return songs_[index];};
        void setPlaylistName(string playlistName){playlistName_ = playlistName;}

        void addSong(const Song& song) override;
        void removeSong(const Song& song) override;
        void listAllPlaylistSongs() const override;

        void moveSong(const int moveFromIndex, const int moveToIndex);
        int getTotalPlaylistDuration() const;

        Playlist operator + (const Playlist& playlist) const;
        Playlist operator + (const Song& song) const;
        Playlist& operator += (const Playlist& playlist);
        Playlist& operator += (const Song& song);
};