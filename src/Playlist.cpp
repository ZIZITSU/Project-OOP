#include "../include/Playlist.hpp"
#include<vector>
#include<string>
#include<iostream>
using namespace std;

Playlist::Playlist(): playlistName_("Uninitialized") {}

Playlist::Playlist(string name){playlistName_ = name;}

Playlist::~Playlist(){cout << "Playlist \"" << playlistName_ << "\" Deleted." << endl;}

void Playlist::addSong(const int songID){
    songIDs_.push_back(songID);
}

void Playlist::removeSong(const int songID){
    for(auto it = songIDs_.begin(); it != songIDs_.end(); ++it){
        if(*it == songID){
            songIDs_.erase(it);
            break;
        }
    }
}

void Playlist::moveSong(const int moveFromIndex, const int moveToIndex){
        int playlistSize = songIDs_.size();
        if(moveFromIndex < 0 || moveFromIndex > playlistSize){
            cout << "Error: Invalid Index.";
            return;
        }
        if(moveToIndex < 0 || moveToIndex > playlistSize){
            cout << "Error: Invalid Index.";
            return;
        }
        int id = songIDs_[moveFromIndex];
        songIDs_.erase(songIDs_.begin() + moveFromIndex);
        songIDs_.insert(songIDs_.begin() + moveToIndex, id);
}

int Playlist::getTotalDuration(const SongLibrary& library) const{
    int totalDuration = 0;
    for(int i=0;i<songIDs_.size();i++){
        const Song* song = library.getSong(songIDs_[i]);
        totalDuration += song->duration();
    }
    return totalDuration;
}

const Song* Playlist::getSong(int index, const SongLibrary& library) const{
    if (index < 0 || index >= songIDs_.size()){
        cout << "Error: Index out of range.";
        return nullptr;
    }
    return library.getSong(songIDs_[index]);
}

void Playlist::listAllSongs(const SongLibrary& library) const{
    for(int i=0;i<songIDs_.size();i++){
        const Song* song = getSong(i, library);
        cout << "\"" << song->title() << "\" by " << song->artist()
        << " ,Duration: " << song->duration() << endl;
    }
}

Playlist Playlist::operator + (const Playlist& playlist) const{}
Playlist Playlist::operator + (const Song& song) const{}
Playlist& Playlist::operator += (const Playlist& playlist){}
Playlist& Playlist::operator += (const Song& song){}
