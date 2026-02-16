#pragma once
#include <string>

class SongLibrary;

class Persistence {
public:
    static bool loadSongsCSV(const std::string& filename, SongLibrary& library);
    static bool saveSongsCSV(const std::string& filename, const SongLibrary& library);
};
