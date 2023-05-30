﻿#pragma once
#include "../Core/Core.h"
#include "../Json/json.hpp"
using json = nlohmann::json;

//------------------------------------------------------------------------------------------------------------------------
class WindowSettings
{
public:
	WindowSettings();
	bool saveToFile(const std::string path);								// сохранине в файл
	bool loadFromFIle(const std::string path);								// загрузка из файла
//------------------------------------------------------------------------------------------------------------------------
public:
	std::string title;														// название программы
	sf::VideoMode resolution;												// расширение экрана
	bool fullscrean;														// полный экран ?
	bool vertycalSync;														// вертикальная синхронизация
	int fps_limit;															// лимит фпс
	sf::ContextSettings contextSettings;									// настройки
	std::vector<sf::VideoMode> videoModes;									// видео режимы

//------------------------------------------------------------------------------------------------------------------------
public:
	int music_volume = 0;
	int sound_volume = 0;
	int camera_speed = 1;
	int zoom_speed = 1;

	bool is_sound_play = false;
	bool is_music_play = false;

	json j;
};
//------------------------------------------------------------------------------------------------------------------------