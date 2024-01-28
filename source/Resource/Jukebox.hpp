#pragma once
#include "../stdafx.h"
#include "Files.hpp"

class Jukebox 
{
public:
    Jukebox(const std::string& path);

    void play();

    void setLooping(const bool loop);

    void setVolume(float volume);
    float getVolume() const;

    void stop();
    void pause();
    void update();

    void skip(int n);
    void rewind();

    void request(const std::string& song);
    void requestAll();
    void clear();

    void shuffle();
    void shuffleRemaining();

    std::size_t songsAvailable()    const;
    std::size_t songsInPlaylist()   const;
    std::size_t songsRemaining()    const;

    std::vector<std::string> getCatalog()   const;
    std::vector<std::string> getPlaylist()  const;

    std::string getCurrent()const;

    sf::SoundSource::Status getStatus() const;

    bool isLooping()    const;
    bool playing()      const;
    bool stopped()      const;
    bool paused()       const;

private:

    sf::Music music;

    std::deque<std::pair<std::string, sf::Music*>> playlist;
    std::map<std::string, std::unique_ptr<sf::Music>> catalog;
    sf::SoundSource::Status status;

    int currentTrackIndex;
    float volume;
    bool looping;

    template <typename T>
    void shuffle_range(T first, T last)
    {
        std::random_device rd;
        auto g = std::mt19937(rd());
        std::shuffle(first, last, g);
    }
};