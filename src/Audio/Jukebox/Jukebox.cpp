#include "Jukebox.hpp"

#include <cassert>
#include <filesystem>

namespace Audio::Music 
{
    Jukebox::Jukebox(const std::string& path) :
        current_track_id(0),
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
                        //LOG_WARN("Jukebox failed to add {}", full_path);
                        continue;
                    }
                    const auto inserted = catalog.emplace(filename, std::move(music));
                    assert(inserted.second);
                }
            }
        }
        else
        {
            //LOG_ERROR("Invalid path: {}",path);
        }
    }

    void Jukebox::play()
    {
        if (playlist.empty())
        {
            assert(status == sf::SoundSource::Stopped);
            return;
        }

        assert(current_track_id >= 0);
        assert(current_track_id < playlist.size());

        setVolume(volume);

        playlist[current_track_id].second->play();
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
        assert(current_track_id >= 0);
        assert(current_track_id < playlist.size());
        playlist[current_track_id].second->setVolume(this->volume);
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

        assert(current_track_id >= 0);
        assert(current_track_id < playlist.size());
        playlist[current_track_id].second->stop();
        status = sf::SoundSource::Stopped;
    }

    void Jukebox::pause()
    {
        if (playlist.empty() || status != sf::SoundSource::Playing)
        {
            return;
        }

        assert(current_track_id >= 0);
        assert(current_track_id < playlist.size());
        playlist[current_track_id].second->pause();
        status = sf::SoundSource::Paused;
    }

    void Jukebox::update()
    {
        {
            if (playlist.empty())
            {
                return;
            }

            assert(current_track_id >= 0);
            assert(current_track_id < playlist.size());

            if (status == sf::SoundSource::Playing
                && playlist[current_track_id].second->getStatus() == sf::SoundSource::Stopped)
            {

                playlist[current_track_id].second->setPlayingOffset(sf::Time::Zero);

                if (current_track_id == playlist.size() - 1)
                {
                    if (looping)
                    {
                        current_track_id = 0;
                        play();
                    }
                    else
                    {
                        stop();
                    }
                }
                else
                {
                    ++current_track_id;
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

        assert(current_track_id >= 0);
        assert(current_track_id < playlist.size());

        const bool was_playing = playing();
        stop();

        if (looping)
        {
            if (n > 0)
            {
                current_track_id = (current_track_id + n) % playlist.size();
            }
            else
            {
                current_track_id = playlist.size() + ((current_track_id + n) % playlist.size());
            }
        }
        else {
            if (n > 0) {
                if (current_track_id + n >= playlist.size())
                {
                    current_track_id = playlist.size() - 1;
                }
                else
                {
                    current_track_id += n;
                }
            }
            else
            {
                if (current_track_id < static_cast<std::size_t>(std::labs(n)))
                {
                    current_track_id = 0;
                }
                else
                {
                    current_track_id += n;
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
        current_track_id = 0;

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
            //LOG_WARN("Jukebox request for song {0} which is not in the catalog.", song);
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
        current_track_id = 0;
    }

    void Jukebox::shuffle()
    {
        if (playlist.empty())
        {
            return;
        }

        assert(current_track_id >= 0);
        assert(current_track_id < playlist.size());

        auto current = playlist[current_track_id];
        if (status != sf::SoundSource::Stopped)
        {
            auto it = begin(playlist);
            std::advance(it, current_track_id);
            playlist.erase(it);
        }

        shuffle_range(begin(playlist), end(playlist));

        if (status != sf::SoundSource::Stopped)
        {
            playlist.emplace_front(current);
        }

        current_track_id = 0;
    }

    void Jukebox::shuffleRemaining()
    {
        if (playlist.empty() || current_track_id == playlist.size() - 1)
        {
            return;
        }

        assert(current_track_id >= 0);
        assert(current_track_id < playlist.size());

        auto first = begin(playlist);
        if (status == sf::SoundSource::Stopped)
        {
            std::advance(first, current_track_id);
        }
        else
        {
            std::advance(first, current_track_id + 1);
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
        return playlist.size() - current_track_id;
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
        assert(current_track_id >= 0);
        assert(current_track_id < playlist.size());
        return playlist[current_track_id].first;
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
}