#include "Jukebox.hpp"

Jukebox::Jukebox(const std::string& path)
    :currentTrackIndex(0)
{
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.path().extension() == ".wav")
        {
            musicFiles.push_back(entry.path().string());
        }
    }
}

bool Jukebox::loadMusic()
{
    if (currentTrackIndex >= 0 && currentTrackIndex < musicFiles.size())
    {
        if (!music.openFromFile(musicFiles[currentTrackIndex]))
        {
            LOG_ERROR("Failed to load music file: {}", musicFiles[currentTrackIndex]);
            return false;
        }
        return true;
    }
    return false;
}

void Jukebox::play(bool repeat = true)
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

void Jukebox::nextTrack()
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

void Jukebox::previousTrack()
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

void Jukebox::repeat()
{
    stop();
    loadMusic();
    play();
}

void Jukebox::setVolume(float volume)
{
    if (volume > 100.0f || volume < 0.0f)
    {
        return;
    }
    music.setVolume(volume);
}

const float Jukebox::getVolume() const
{
    return music.getVolume();
}

void Jukebox::stop()
{
    music.stop();
}
