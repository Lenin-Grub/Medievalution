#pragma once
#include "../Core/Core.h"
#include "../Json/json.hpp"
#include "../Localisation/Localisation.hpp"

using json = nlohmann::json;

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

	bool saveToFile  (const std::string path) noexcept;
	bool loadFromFIle(const std::string path) noexcept;

	std::string                language;
	std::string                title;
	std::vector<sf::VideoMode> video_modes;
	sf::VideoMode              resolution;
	sf::ContextSettings        context_settings;
	json                       localisation;

	int                        fps_limit;
	int                        id_resolution;
	int                        music_volume;
	int                        sound_volume;
	int                        camera_speed;
	bool                       fullscrean;
	bool                       vertycal_sync;
	bool                       is_sound_play;
	bool                       is_music_play;
	float                      zoom_speed;

};
