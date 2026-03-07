#include "Persistence.hpp"
#include "Playlist.hpp"
#include "Song.hpp"
#include "SongLibrary.hpp"
#include <iostream>


int main(){
    SongLibrary library;
    Persistence::loadSongsCSV("songs.csv", library);

    Playlist p1(library, "Test Playlist");
    Playlist p2(library);
    Playlist p3(library, "p33");
    try{
        p1.addSong(*library.getSong(12));
        p1.addSong(*library.getSong(10));
        p1.addSong(*library.getSong(5));
        p1.addSong(*library.getSong(6));
        p1.addSong(*library.getSong(7));
        p1.addSong(30);
        p1.listAllPlaylistSongs();
        cout << "1------------------------\n";
        p1.removeSong(7);
        p1.listAllPlaylistSongs();
        cout << "2------------------------\n";
        p1 += 25;
        p1 += 26;
        p1 += *library.getSong(27);
        p1.listAllPlaylistSongs();
        cout << "3------------------------\n";
        p2.addSong(33);
        p2.addSong(34);
        p2.addSong(35);
        p2.addSong(36);
        p3 = p1 + 37;
        p3.listAllPlaylistSongs();
    }catch(SongNotFoundException& e){
        cerr << e.what() << "\n";
    }catch(DuplicateSongExeption& e){
        cerr << e.what() << "\n";
    }catch(InvalidIndexException& e){
        cerr << e.what() << "\n";
    }catch(PlaylistException& e){
        cerr << e.what() << "\n";
    }
}