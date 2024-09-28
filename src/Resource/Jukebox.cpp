#include "Jukebox.hpp"

Jukebox::Jukebox(const std::string& path) :
    currentTrackIndex(0),
    volume(20),
    status(sf::SoundSource::Stopped),
    looping(true)
{
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.path().extension() == ".wav")
        {
            std::string directory = path;
            std::string filename = entry.path().filename().string();
            std::string full_path = directory + filename;

            auto music = std::unique_ptr<sf::Music>(new sf::Music);
            if (!music->openFromFile(full_path))
            {
                LOG_ERROR("Jukebox failed to add {}", full_path);
                continue;
            }
            const auto inserted = catalog.emplace(filename, std::move(music));
            assert(inserted.second);
        }
    }
    }
    else
    {
     LOG_ERROR("Invalid path: {}",path);
    }
}

void Jukebox::play()
{
    if (playlist.empty()) 
    {
        assert(status == sf::SoundSource::Stopped);
        return;
    }

    assert(currentTrackIndex >= 0);
    assert(currentTrackIndex < playlist.size());

    setVolume(getVolume());

    playlist[currentTrackIndex].second->play();
    status = sf::SoundSource::Playing;
}

void Jukebox::setVolume(float volume)
{
    this->volume = volume;
    if (this->volume < 0.f) 
    {
        this->volume = 0.f;
    }
    else if (this->volume > 100.f)
    {
        this->volume = 100.f;
    }
    if (this->playlist.empty())
    {
        return;
    }
    assert(currentTrackIndex >= 0);
    assert(currentTrackIndex < playlist.size());
    playlist[currentTrackIndex].second->setVolume(this->volume);
}

float Jukebox::getVolume() const
{
    return music.getVolume();
}

void Jukebox::stop()
{
    if (playlist.empty() || status == sf::SoundSource::Stopped) 
    {
        return;
    }

    assert(currentTrackIndex >= 0);
    assert(currentTrackIndex < playlist.size());
    playlist[currentTrackIndex].second->stop();
    status = sf::SoundSource::Stopped;
}

void Jukebox::pause()
{
    if (playlist.empty() || status != sf::SoundSource::Playing) 
    {
        return;
    }

    assert(currentTrackIndex >= 0);
    assert(currentTrackIndex < playlist.size());
    playlist[currentTrackIndex].second->pause();
    status = sf::SoundSource::Paused;
}

void Jukebox::update()
{
    {
        if (playlist.empty()) 
        {
            return;
        }

        assert(currentTrackIndex >= 0);
        assert(currentTrackIndex < playlist.size());

        if (status == sf::SoundSource::Playing
            && playlist[currentTrackIndex].second->getStatus() == sf::SoundSource::Stopped) 
        {

            playlist[currentTrackIndex].second->setPlayingOffset(sf::Time::Zero);

            if (currentTrackIndex == playlist.size() - 1) 
            {
                if (looping)
                {
                    currentTrackIndex = 0;
                    play();
                }
                else 
                {
                    stop();
                }
            }
            else 
            {
                ++currentTrackIndex;
                play();
            }
        }
    }
}

void Jukebox::skip(int n)
{
    if (playlist.empty())
    {
        return;
    }

    assert(currentTrackIndex >= 0);
    assert(currentTrackIndex < playlist.size());

    const bool was_playing = playing();
    stop();

    if (looping)
    {
        if (n > 0)
        {
            currentTrackIndex = (currentTrackIndex + n) % playlist.size();
        }
        else
        {
            currentTrackIndex = playlist.size() + ((currentTrackIndex + n) % playlist.size());
        }
    }
    else {
        if (n > 0) {
            if (currentTrackIndex + n >= playlist.size())
            {
                currentTrackIndex = playlist.size() - 1;
            }
            else
            {
                currentTrackIndex += n;
            }
        }
        else
        {
            if (currentTrackIndex < static_cast<std::size_t>(std::labs(n)))
            {
                currentTrackIndex = 0;
            }
            else
            {
                currentTrackIndex += n;
            }
        }
    }
    if (was_playing)
    {
        play();
    }
}

void Jukebox::rewind()
{
    auto orig_status = status;
    stop();
    currentTrackIndex = 0;

    if (orig_status == sf::SoundSource::Playing)
    {
        play();
    }
}

void Jukebox::request(const std::string& song)
{
    auto found = catalog.find(song);
    if (found == catalog.end())
    {
        LOG_ERROR("Jukebox request for song {0} which is not in the catalog.", song);
        return;
    }
    playlist.emplace_back(found->first, found->second.get());
}

void Jukebox::requestAll()
{
    for (const auto& song : catalog)
    {
        playlist.emplace_back(song.first, song.second.get());
    }
}

void Jukebox::clear()
{
    stop();
    playlist.clear();
    currentTrackIndex = 0;
}

void Jukebox::shuffle()
{
    if (playlist.empty())
    {
        return;
    }

    assert(currentTrackIndex >= 0);
    assert(currentTrackIndex < playlist.size());

    auto current = playlist[currentTrackIndex];
    if (status != sf::SoundSource::Stopped)
    {
        auto it = begin(playlist);
        std::advance(it, currentTrackIndex);
        playlist.erase(it);
    }

    shuffle_range(begin(playlist), end(playlist));

    if (status != sf::SoundSource::Stopped)
    {
        playlist.emplace_front(current);
    }

    currentTrackIndex = 0;
}

void Jukebox::shuffleRemaining()
{
    if (playlist.empty() || currentTrackIndex == playlist.size() - 1)
    {
        return;
    }

    assert(currentTrackIndex >= 0);
    assert(currentTrackIndex < playlist.size());

    auto first = begin(playlist);
    if (status == sf::SoundSource::Stopped)
    {
        std::advance(first, currentTrackIndex);
    }
    else
    {
        std::advance(first, currentTrackIndex + 1);
    }

    shuffle_range(first, end(playlist));
}

void Jukebox::setLooping(const bool loop)
{
    looping = loop;
}

bool Jukebox::isLooping() const
{
    return looping;
}

std::size_t Jukebox::songsAvailable() const
{
    return catalog.size();
}

std::size_t Jukebox::songsInPlaylist() const
{
    return playlist.size();
}

std::size_t Jukebox::songsRemaining() const
{
    return playlist.size() - currentTrackIndex;
}

std::vector<std::string> Jukebox::getCatalog() const
{
    std::vector<std::string> retval;
    for (const auto& song : catalog)
    {
        retval.emplace_back(song.first);
    }
    return retval;
}

std::vector<std::string> Jukebox::getPlaylist() const
{
    std::vector<std::string> retval;
    for (const auto& song : playlist)
    {
        retval.emplace_back(song.first);
    }
    return retval;
}

std::string Jukebox::getCurrent() const
{
    if (playlist.empty())
    {
        return "";
    }
    assert(currentTrackIndex >= 0);
    assert(currentTrackIndex < playlist.size());
    return playlist[currentTrackIndex].first;
}

sf::SoundSource::Status Jukebox::getStatus() const
{
    return status;
}

bool Jukebox::playing() const
{
    return status == sf::SoundSource::Playing;
}

bool Jukebox::stopped() const
{
    return status == sf::SoundSource::Stopped;
}

bool Jukebox::paused() const
{
    return status == sf::SoundSource::Paused;
}
