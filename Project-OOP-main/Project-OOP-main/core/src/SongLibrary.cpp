#include "SongLibrary.hpp"
#include <cctype>
#include <string>
#include <vector>


using namespace std; 

bool SongLibrary::addSong(const Song& s)
{
    return songs_.emplace(s.id(), s).second;
}

Song* SongLibrary::getSong(int id){
    auto it = songs_.find(id);
    if (it == songs_.end()){
        return nullptr;
    }

    return &it->second;
}


const Song* SongLibrary::getSong(int id) const{
    auto it = songs_.find(id);
    if (it == songs_.end()){
        return nullptr;
    }

    return &it->second;
}

vector<const Song*> SongLibrary::getAllSongs() const{
    vector<const Song*> result;
    result.reserve(songs_.size());

    for (const auto& p : songs_){
        result.push_back(&p.second);
    }

    return result;
}




static string toLowerCopy(string s)
{
    for (char& c : s)
        c = (char)tolower((unsigned char)c);
    return s;
}

vector<const Song*> SongLibrary::searchTitle(const string& keyword) const
{
    vector<const Song*> result;
    string key = toLowerCopy(keyword);

    for (const auto& p : songs_)
    {
        string t = toLowerCopy(p.second.title());
        if (t.find(key) != string::npos)
            result.push_back(&p.second);
    }
    return result;
}

vector<const Song*> SongLibrary::searchArtist(const string& keyword) const
{
    vector<const Song*> result;
    string key = toLowerCopy(keyword);

    for (const auto& p : songs_)
    {
        string a = toLowerCopy(p.second.artist());
        if (a.find(key) != string::npos)
            result.push_back(&p.second);
    }
    return result;
}
