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
        virtual ~BasePlaylist(){}
};

class Playlist: public BasePlaylist{
    protected:
        string playlistName_;
        vector<const Song*> songs_;
        int playlistID_;
        static int currentPlaylistID;
        const SongLibrary* library_;
    public:
        Playlist(const SongLibrary& library);
        Playlist(const SongLibrary& library, string name);
        virtual ~Playlist();

        string getPlaylistName() const{return playlistName_;}
        int getPlaylistID() const{return playlistID_;}
        const Song* getSongAtIndex(int index) const{return songs_[index];};
        int getPlaylistSize() const{return songs_.size();}
        void setPlaylistName(string playlistName){playlistName_ = playlistName;}

        void addSong(const Song& song) override;
        void addSong(const int songID);
        void removeSong(const Song& song) override;
        void removeSong(const int songID);
        void listAllPlaylistSongs() const override;

        void moveSong(const int moveFromIndex, const int moveToIndex);
        int getTotalPlaylistDuration() const;

        Playlist operator + (const Playlist& playlist) const;
        Playlist operator + (const Song& song) const;
        Playlist& operator += (const Playlist& playlist);
        Playlist& operator += (const Song& song);
};

class FavoritePlaylist: public Playlist{
    private:
        vector<const Song*> favorites;
    public:
        FavoritePlaylist(const SongLibrary& library);
        void addSong(const Song& song) override;
        void addSong(const int songID);
        void removeSong(const Song& song) override;
        void removeSong(const int songID);
};


//Exceptions

class PlaylistException{
    protected:
        string message_;
    public:
        PlaylistException(): message_("Playlist error.");
        PlaylistException(const string message): message_(message) {}
        string what() const{return message_;}
};

class InvalidIndexException: public PlaylistException{
    public:
        InvalidIndexException(): PlaylistException("Index out of range.") {}
        InvalidIndexException(const string message): PlaylistException(message) {} 
};

class SongNotFoundException: public PlaylistException{
    public:
        SongNotFoundException(): PlaylistException("Song not found.") {}
        SongNotFoundException(const string message): PlaylistException(message) {}
};

class DuplicateSongExeption: public PlaylistException{
    public:
        DuplicateSongExeption(): PlaylistException("Song already included.") {}
        DuplicateSongExeption(const string message): PlaylistException(message) {}
};