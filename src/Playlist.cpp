#include "../include/Playlist.hpp"
#include<vector>
#include<string>
#include<iostream>
using namespace std;

//----Playlist----

int Playlist::currentPlaylistID = 1; //Initializing static member

//Playlist cannot be created without any library
Playlist::Playlist(const SongLibrary& library): playlistName_("Uninitialized"), library_(&library){
    playlistID_ = currentPlaylistID++;
}

Playlist::Playlist(const SongLibrary& library, string name): playlistName_(name), library_(&library){
    playlistID_ = currentPlaylistID++;
}

Playlist::~Playlist(){cout << "Playlist \"" << playlistName_ << "\" Deleted." << endl;}

//Add or remove song, can be done in two ways: through a reference, or through the songID.
void Playlist::addSong(const Song& song){
    const Song* temp = library_->getSong(song.id());
    if(!temp) throw SongNotFoundException();
    for(int i=0;i<songs_.size();i++){
        if(songs_[i]->id() == song.id()) throw DuplicateSongExeption(song.title() + " already inlcuded.");
    }
    songs_.push_back(temp);
}

void Playlist::addSong(const int songID){
    const Song* temp = library_->getSong(songID);
    if(!temp) throw SongNotFoundException("Song " + to_string(songID) + " not found.");
    addSong(*temp);
}

void Playlist::removeSong(const Song& song){
    if(!library_->getSong(song.id())) throw SongNotFoundException();
    for(auto temp = songs_.begin(); temp != songs_.end(); ++temp){
        if((*temp)->id() == song.id()){
            songs_.erase(temp);
            return;
        }
    }
    throw SongNotFoundException(song.title() + " not found in playlist.");
}

void Playlist::removeSong(const int songID){
    const Song* temp = library_->getSong(songID);
    if(!temp) throw SongNotFoundException();
    removeSong(*temp);
}

//Move the position of a song within the playlist
void Playlist::moveSong(const int moveFromIndex, const int moveToIndex){
    int playlistSize = songs_.size();
    if(moveFromIndex < 0 || moveFromIndex > playlistSize){
        throw InvalidIndexException(to_string(moveFromIndex) + " out of range.");
    }
    if(moveToIndex < 0 || moveToIndex > playlistSize){
        throw InvalidIndexException(to_string(moveToIndex) + " out of range.");
    }
    const Song* temp = songs_[moveFromIndex];
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
    if(songs_.empty()){
        cout << "Playlist is empty.";
        return;
    }
    for(int i=0;i<songs_.size();i++){
        const Song* song = songs_[i];
        cout << "\"" << song->title() << "\" by " << song->artist()
        << ", Duration: " << song->duration() << " secs" << endl;
    }
}

// '+' operator adds a song/playlist and returns the playlist
Playlist Playlist::operator + (const Playlist& playlist) const{
    Playlist temp = *this;
    temp.setPlaylistName("Merged: " + playlistName_ + " + " + playlist.playlistName_);
    for(int i=0;i<playlist.songs_.size();i++){
        temp.addSong(*playlist.songs_[i]);
    }
    return temp;
}

Playlist Playlist::operator + (const Song& song) const{
    Playlist temp = *this;
    temp.setPlaylistName(playlistName_  + "+");
    temp.addSong(song);
    return temp;
}

Playlist Playlist::operator + (const int songID) const{
    Playlist temp = *this;
    temp.setPlaylistName(playlistName_  + "+");
    temp.addSong(songID);
    return temp;
}

// '+=' operator adds a song to the current playlist
Playlist& Playlist::operator += (const Playlist& playlist){
    for(int i=0;i<playlist.songs_.size();i++){
        songs_.push_back(playlist.songs_[i]);
    }
    return *this;
}

Playlist& Playlist::operator += (const Song& song){
    this->addSong(song);
    return *this;
}

Playlist& Playlist::operator += (const int songID){
    this->addSong(songID);
    return *this;
}

//copies playlist
Playlist& Playlist::operator = (const Playlist& playlist) {
    if (this == &playlist) return *this;
    playlistName_ = playlist.playlistName_;
    songs_        = playlist.songs_;
    library_      = playlist.library_;
    return *this;
}






//----FavoritePlaylist----

FavoritePlaylist::FavoritePlaylist(const SongLibrary& library): Playlist(library, "Favourites") {}

void FavoritePlaylist::addSong(const Song& song){
    const Song* temp = library_->getSong(song.id());
    if(!temp) throw SongNotFoundException();
    for(int i=0;i<songs_.size();i++){
        if(songs_[i]->id() == song.id()) throw DuplicateSongExeption(song.title() + " already inlcuded.");
    }
    favorites.push_back(temp);
}

void FavoritePlaylist::addSong(const int songID){
    const Song* temp = library_->getSong(songID);
    if(!temp) throw SongNotFoundException();
    addSong(*temp);
}

void FavoritePlaylist::removeSong(const Song& song){
    if(!library_->getSong(song.id())) throw SongNotFoundException();
    for(auto it = favorites.begin(); it != favorites.end(); ++it){
        if((*it)->id() == song.id()){
            favorites.erase(it);
            return;
        }
    }
    throw SongNotFoundException(song.title() + " not found in playlist.");
}

void FavoritePlaylist::removeSong(const int songID){
    const Song* temp = library_->getSong(songID);
    if(!temp) throw SongNotFoundException();
    removeSong(*temp);
}