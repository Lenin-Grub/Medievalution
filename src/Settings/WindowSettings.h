#pragma once
#include "../Core/Core.h"
#include "../Json/json.hpp"
#include "../Localisation/Localisation.hpp"

using json = nlohmann::json;

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
	bool saveToFile(const std::string path)		noexcept;

/// @brief Loads the window settings from a file
/// This method loads the window settings from a specified file
/// @param path The path to the file from which settings will be loaded.
/// @return True if the settings were successfully loaded; false otherwise.
	bool loadFromFIle(const std::string path)	noexcept;

public:
    std::string                title;                          ///< The window title.
    sf::VideoMode              resolution;                     ///< The window resolution.
    bool                       fullscreen;                     ///< Flag indicating whether fullscreen mode is enabled.
    bool                       vertical_sync;                  ///< Flag indicating whether vertical sync is enabled.
    int                        fps_limit;                      ///< The frame rate limit.
    sf::ContextSettings        context_settings;               ///< OpenGL context settings.
    std::vector<sf::VideoMode> video_modes;                    ///< Available video modes for the window.
    json                       localization;                   ///< Localization settings in JSON format.
    int                        id_resolution;                  ///< ID of the currently selected resolution.
    int                        music_volume;                   ///< Volume level for the music.
    int                        sound_volume;                   ///< Volume level for sound effects.
    int                        camera_speed;                   ///< Speed of the camera movement.
    float                      zoom_speed;                     ///< Speed of zooming.
    bool                       is_sound_play;                  ///< Flag indicating whether sound effects are playing.
    bool                       is_music_play;                  ///< Flag indicating whether music is playing.
    std::string                language;                       ///< The language setting.
};
