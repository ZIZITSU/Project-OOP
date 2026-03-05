#include "Persistence.hpp"
#include "SongLibrary.hpp"
#include "Song.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

static std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) out.push_back(item);
    return out;
}

bool Persistence::loadSongsCSV(const std::string& filename, SongLibrary& library)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open: " << filename << "\n";
        return false;
    }

    std::string line;
    std::getline(file, line); // header

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> cols;

        while (std::getline(ss, field, ',')) cols.push_back(field);
        if (cols.size() < 9) continue;

        int id = std::stoi(cols[0]);
        std::string title = cols[1];
        std::string artist = cols[2];
        std::string album = cols[3];
        int duration = std::stoi(cols[4]);
        std::string genre = cols[5];
        auto moods = split(cols[6], '|');

        int rating = std::stoi(cols[7]);
        int playCount = std::stoi(cols[8]);

        Song s(id, title, artist, album, duration, genre, moods);
        if (rating > 0) s.rate(rating);
        for (int i = 0; i < playCount; i++) s.incrementPlay();

        library.addSong(s);
    }

    return true;
}

static std::string join(const std::vector<std::string>& v, char delim)
{
    std::string out;
    for (size_t i = 0; i < v.size(); i++)
    {
        out += v[i];
        if (i + 1 < v.size()) out += delim;
    }
    return out;
}

bool Persistence::saveSongsCSV(const std::string& filename, const SongLibrary& library)
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        std::cerr << "Failed to write: " << filename << "\n";
        return false;
    }

    out << "id,title,artist,album,duration,genre,moods,rating,playCount\n";

    auto songs = library.getAllSongs();
    for (const Song* s : songs)
    {
        out << s->id() << ","
            << s->title() << ","
            << s->artist() << ","
            << s->album() << ","
            << s->duration() << ","
            << s->genre() << ","
            << join(s->moods(), '|') << ","
            << s->rating() << ","
            << s->playCount() << "\n";
    }

    return true;
}
