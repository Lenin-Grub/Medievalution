#pragma once
#include "../stdafx.h"

/**
* @class Jukebox
* @brief A class to manage and play a collection of music tracks.
*
* The Jukebox class is responsible for handling music playback, including
* playing, pausing, stopping, shuffling, and managing a playlist of tracks.
*/
class Jukebox 
{
public:
    Jukebox(const std::string& path);

/// @brief Starts playing the current track in the playlist.
    void play();

/// @brief Sets whether the music should loop.
/// @param loop If true, the music will loop.
    void setLooping(const bool loop);

/// @brief Sets the volume of the music.
/// @param The volume level to set(0.0 to 100.0).
    void setVolume(float volume);

/// @brief Gets the current volume of the music
/// @return The current volume level(0.0 to 100.0).
    float getVolume() const;

/// @brief Stops the music playback.
    void stop();
/// @brief Pauses the music playback.
    void pause();
/// @brief Updates the jukebox, typically called each frame.
    void update();

/// @brief Skips the specified number of tracks.
/// @param n The number of tracks to skip.
    void skip(int n);

/// @brief Rewinds to the beginning of the current track.
    void rewind();

/// @brief Requests to play the specified song.
/// @param song The name of the song to request.
    void request(const std::string& song);

/// @brief Requests to play all songs in the jukebox.
    void requestAll();

/// @brief Clears the current playlist.
    void clear();

/// @brief Shuffles the current playlist.
    void shuffle();

/// @brief Shuffles the remaining tracks in the playlist.
    void shuffleRemaining();

/// @brief Gets the number of songs available in the catalog.
/// @return The number of available songs.
    std::size_t songsAvailable() const;

/// @brief Gets the number of songs currently in the playlist.
/// @return The number of songs in the playlist.
    std::size_t songsInPlaylist() const;

/// @brief Gets the number of remaining songs in the playlist.
/// @return The number of remaining songs.
    std::size_t songsRemaining() const;

/// @brief Gets the catalog of available songs.
/// @return A vector of available song names.
    std::vector<std::string> getCatalog() const;

/// @brief Gets the current playlist.
/// @return A vector of song names in the playlist.
    std::vector<std::string> getPlaylist()const;

/// @brief Gets the name of the currently playing song.
/// @return The name of the current song.
    std::string getCurrent() const;

/// @brief Gets the current status of the music playback
/// @return The status(Playing, Paused, or Stopped).
    sf::SoundSource::Status getStatus()const;

/// @brief Checks if the music is set to loop
    bool isLooping() const;

/// @brief Checks if music is currently playing.
    bool playing() const;

/// @brief Checks if music playback is stopped.
    bool stopped() const;

/// @brief Checks if music playback is paused.
    bool paused() const;

private:

    sf::Music                                         music;             ///< The SFML music object that handles playback.
    std::deque<std::pair<std::string, sf::Music*>>    playlist;          ///< The playlist queue.
    std::map<std::string, std::unique_ptr<sf::Music>> catalog;           ///< The catalog of available songs.
    sf::SoundSource::Status                           status;            ///< The current status of the music playback.
    int                                               currentTrackIndex; ///< Index of the current track in the playlist.
    float                                             volume;            ///< The current volume level.
    bool                                              looping;           ///< Whether the music should loop.

/// @brief Shuffles a range of elements.
    template <typename T>
    void shuffle_range(T first, T last)
    {
        std::random_device rd;
        auto g = std::mt19937(rd());
        std::shuffle(first, last, g);
    }
};