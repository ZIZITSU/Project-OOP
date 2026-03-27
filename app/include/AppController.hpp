#ifndef APPCONTROLLER_HPP
#define APPCONTROLLER_HPP

#include <QObject>
#include <QString>
#include <QVector>
#include "SongLibrary.hpp"
#include <memory>

using namespace std;


// AppController - Main application controller (MVC pattern) - It Connects the GUI to the backend logic

class AppController : public QObject {
    Q_OBJECT
    
public:
    explicit AppController(QObject *parent = nullptr);
    ~AppController();
    
    // Song library management
    bool loadSongsFromCSV(const QString& path);
    int getSongCount() const;
    
    // Getting songs for display
    vector<shared_ptr<Song>> getAllSongs() const;
    vector<shared_ptr<Song>> searchSongs(const QString& query) const;
    vector<shared_ptr<Song>> filterByMood(const QString& mood) const;
    
    // Getting song by ID
    shared_ptr<Song> getSongById(int id) const;
    
signals:
    void songsLoaded();
    void songCountChanged(int count);
    
private:
    unique_ptr<SongLibrary> m_library;
};

#endif 