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

	bool saveToFile(const std::string path)		noexcept;
	bool loadFromFIle(const std::string path)	noexcept;

public:
	std::string title;
	sf::VideoMode resolution;
	bool fullscrean;
	bool vertycal_sync;
	int fps_limit;
	sf::ContextSettings context_settings;
	std::vector<sf::VideoMode> video_modes;
	json localisation;

	int id_resolution;
	int music_volume;
	int sound_volume;
	int camera_speed;
	float zoom_speed;

	bool is_sound_play;
	bool is_music_play;

	std::string language;
};
