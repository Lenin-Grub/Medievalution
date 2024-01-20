#pragma once
#include "../stdafx.h"
#include "Files.hpp"

class Jukebox 
{
public:
    Jukebox(const std::string& path);

    bool loadMusic();

    void play();

    void nextTrack();

    void previousTrack();

    void repeat();

    void setVolume(float volume);

    const float getVolume() const;

    void stop();

private:

    sf::Music music;
    std::vector<std::string> musicFiles;

    int currentTrackIndex;
    float volume;
};