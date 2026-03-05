#include "../include/Playlist.hpp"
#include<vector>
#include<string>
#include<iostream>
using namespace std;

int Playlist::currentPlaylistID = 1;

Playlist::Playlist(const SongLibrary& library): playlistName_("Uninitialized"), library_(&library) {
    playlistID_ = currentPlaylistID++;
    
}

Playlist::Playlist(const SongLibrary& library, string name): playlistName_(name), library_(&library){
    playlistID_ = currentPlaylistID++;
}

Playlist::~Playlist(){cout << "Playlist \"" << playlistName_ << "\" Deleted." << endl;}

void Playlist::addSong(const Song& song){
    Song* temp = library_->getSong(song.id());
    songs_.push_back(temp);
}

void Playlist::removeSong(const Song& song){
    for(auto temp = songs_.begin(); temp != songs_.end(); ++temp){
        if((*temp)->id() == song.id()){
            songs_.erase(temp);
        }
    }
}

void Playlist::moveSong(const int moveFromIndex, const int moveToIndex){
    int playlistSize = songs_.size();
    if(moveFromIndex < 0 || moveFromIndex > playlistSize){
        cout << "Error: Invalid Index.";
        return;
    }
    if(moveToIndex < 0 || moveToIndex > playlistSize){
        cout << "Error: Invalid Index.";
        return;
    }
    Song* temp = songs_[moveFromIndex];
    songs_.erase(songs_.begin() + moveFromIndex);
    songs_.insert(songs_.begin() + moveToIndex, temp);
}

int Playlist::getTotalPlaylistDuration() const{
    int totalDuration = 0;
    for(int i=0;i<songs_.size();i++){
        totalDuration += songs_[i]->duration();
    }
    return totalDuration;
}

void Playlist::listAllPlaylistSongs() const{
    for(int i=0;i<songs_.size();i++){
        const Song* song = songs_[i];
        cout << "\"" << song->title() << "\" by " << song->artist()
        << " ,Duration: " << song->duration() << endl;
    }
}

Playlist Playlist::operator + (const Playlist& playlist) const{}
Playlist Playlist::operator + (const Song& song) const{}
Playlist& Playlist::operator += (const Playlist& playlist){}
Playlist& Playlist::operator += (const Song& song){}
