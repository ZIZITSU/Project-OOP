#include "SongLibrary.hpp"
#include "Persistence.hpp"
#include <iostream>

int main()
{
    SongLibrary library;
    if (!Persistence::loadSongsCSV("data/songs.csv", library))
    {
        std::cout << "Failed to load CSV\n";
        return 1;
    }

    std::cout << "Loaded " << library.getAllSongs().size() << " songs\n";
    return 0;
}
