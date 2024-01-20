#pragma once
#include "../stdafx.h"
#include "Files.hpp"
//
//class Jukebox
//{
//public:
//	Jukebox();
//	~Jukebox();
//
//	void next();
//	void prev();
//	void stop();
//	void play();
//	void repeat();
//	void toggle();
//
//	void setVolume(float volume);
//	const float getVolume() const;
//private:
//};

class Jukebox 
{
private:
    std::vector<std::string> musicFiles;
    sf::Music music;
    int currentTrackIndex;
    float volume;

public:
    Jukebox(const std::string& path) 
    {
        // Получаем все файлы с расширением .mp3 в указанной папке
        for (const auto& entry : std::filesystem::directory_iterator(path)) 
        {
            if (entry.path().extension() == ".wav") 
            {
                musicFiles.push_back(entry.path().string());
            }
        }

        currentTrackIndex = 0;
        volume = 100;
    }

    bool loadMusic() 
    {
        if (currentTrackIndex >= 0 && currentTrackIndex < musicFiles.size()) 
        {
            if (!music.openFromFile(musicFiles[currentTrackIndex])) 
            {
                LOG_ERROR("Failed to load music file: {}", musicFiles[currentTrackIndex]);
                return false;
            }
            music.setVolume(volume);
            return true;
        }
        return false;
    }

    void play() 
    {
        if (music.getStatus() == sf::Music::Playing)
            return;

        if (musicFiles.empty()) 
        {
            LOG_ERROR("Music file not found!");
        }
        else 
        {
            if (loadMusic()) 
            {
                music.play();
                LOG_INFO("Playing music file: {}", musicFiles[currentTrackIndex]);
            }
        }
    }

    void playAll() 
    {
        if (music.getStatus() == sf::Music::Playing)
            return;

        if (music.getStatus() == sf::Music::Stopped)
        {
            if (currentTrackIndex == musicFiles.size())
            {
                currentTrackIndex = 0;
                play();
            }
            else
            {
                nextTrack();
            }
        }
    }

    void nextTrack() 
    {
        if (currentTrackIndex < musicFiles.size() - 1) 
        {
            ++currentTrackIndex;
        }
        else 
        {
            currentTrackIndex = 0;
        }

        stop();
        loadMusic();
        play();
    }

    void previousTrack() 
    {
        if (currentTrackIndex > 0) 
        {
            --currentTrackIndex;
        }
        else 
        {
            currentTrackIndex = musicFiles.size() - 1;
        }

        stop();
        loadMusic();
        play();
    }

    void repeat() 
    {
        stop();
        loadMusic();
        play();
    }

    void increaseVolume() 
    {
        if (volume < 100) 
        {
            volume += 10;
            music.setVolume(volume);
        }
    }

    void decreaseVolume() 
    {
        if (volume > 0) 
        {
            volume -= 10;
            music.setVolume(volume);
        }
    }

    void stop() {
        music.stop();
    }
};