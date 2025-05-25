#pragma once

#include <SFML/System.hpp>
#include <Common/Common.h>
#include <json.hpp>
#include <fstream>
#include <Common/Log.h>

using json = nlohmann::json;

struct Settings
{
    std::string                title;             ///< @brief The window title.
    sf::VideoMode              resolution;        ///< @brief The window resolution.
    bool                       fullscreen;        ///< @brief Flag indicating whether fullscreen mode is enabled.
    bool                       vertical_sync;     ///< @brief Flag indicating whether vertical sync is enabled.
    int                        fps_limit;         ///< @brief The frame rate limit.
    sf::ContextSettings        context_settings;  ///< @brief OpenGL context settings.
    std::vector<sf::VideoMode> video_modes;       ///< @brief Available video modes for the window.
    int                        id_resolution;     ///< @brief ID of the currently selected resolution.
    int                        music_volume;      ///< @brief Volume level for the music.
    int                        sound_volume;      ///< @brief Volume level for sound effects.
    int                        camera_speed;      ///< @brief Speed of the camera movement.
    float                      zoom_speed;        ///< @brief Speed of zooming.
    bool                       is_sound_play;     ///< @brief Flag indicating whether sound effects are playing.
    bool                       is_music_play;     ///< @brief Flag indicating whether music is playing.
    std::string                language;          ///< @brief The language setting.

    bool operator==(const Settings& other) const 
    {
            return title                       == other.title                               &&
            resolution                         == other.resolution                          &&
            fullscreen                         == other.fullscreen                          &&
            vertical_sync                      == other.vertical_sync                       &&
            fps_limit                          == other.fps_limit                           &&
            context_settings.depthBits         == other.context_settings.depthBits          &&
            context_settings.stencilBits       == other.context_settings.stencilBits        &&
            context_settings.antialiasingLevel == other.context_settings.antialiasingLevel  &&
            context_settings.majorVersion      == other.context_settings.majorVersion       &&
            context_settings.minorVersion      == other.context_settings.minorVersion       &&
            video_modes                        == other.video_modes                         &&
            id_resolution                      == other.id_resolution                       &&
            music_volume                       == other.music_volume                        &&
            sound_volume                       == other.sound_volume                        &&
            camera_speed                       == other.camera_speed                        &&
            zoom_speed                         == other.zoom_speed                          &&
            is_sound_play                      == other.is_sound_play                       &&
            is_music_play                      == other.is_music_play                       &&
            language                           == other.language;
    }

    bool operator!=(const Settings& other) const 
    {
        return !(*this == other);
    }
};

/**
 * @class WindowSettings
 * @brief A singleton class for managing window settings within an application.
 *
 * The WindowSettings class provides methods to save and load window settings to and from a file.
 * It also contains various settings such as resolution, fullscreen mode, vertical sync, and more.
 */
class WindowSettings
{
private:
    WindowSettings();

public:
    static WindowSettings& getInstance()
    {
        static WindowSettings single_instance;
        return single_instance;
    }

/// @brief Saves the window settings to a file
/// This method saves the current window settings to a specified file
/// @param path The path to the file where settings will be saved.
/// @return True if the settings were successfully saved; false otherwise.
    bool saveToFile(const std::string path) noexcept;

/// @brief Loads the window settings from a file
/// This method loads the window settings from a specified file
/// @param path The path to the file from which settings will be loaded.
/// @return True if the settings were successfully loaded; false otherwise.
    bool loadFromFIle(const std::string path) noexcept;

    float getFPS();

public:
    Settings settings;

private:
    sf::Time lastFrameTime;
    float    fps;
};
