﻿#pragma once
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

	bool saveToFile(const std::string path);								
	bool loadFromFIle(const std::string path);	

public:
	std::string title;														
	sf::VideoMode resolution;												
	bool fullscrean;														
	bool vertycalSync;														
	int fps_limit;															
	sf::ContextSettings contextSettings;									
	std::vector<sf::VideoMode> videoModes;									
	json localisation;

	int music_volume = 0;
	int sound_volume = 0;
	int camera_speed = 1;
	float zoom_speed = 0;

	bool is_sound_play = false;
	bool is_music_play = false;

	std::string language;
};
