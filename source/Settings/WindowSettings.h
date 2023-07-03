#pragma once
#include "../Core/Core.h"
#include "../Json/json.hpp"
using json = nlohmann::json;

//------------------------------------------------------------------------------------------------------------------------
class WindowSettings
{
public:
	static WindowSettings& getInstance()
	{
		static WindowSettings single_instance;
		return single_instance;
	}

	bool saveToFile(const std::string path);								// сохранине в файл
	bool loadFromFIle(const std::string path);								// загрузка из файла
private:
	WindowSettings();
	WindowSettings(const WindowSettings& root) = delete;
	WindowSettings& operator=(const WindowSettings&) = delete;

//------------------------------------------------------------------------------------------------------------------------
public:
	std::string title;														// название программы
	sf::VideoMode resolution;												// расширение экрана
	bool fullscrean;														// полный экран ?
	bool vertycalSync;														// вертикальная синхронизация
	int fps_limit;															// лимит фпс
	sf::ContextSettings contextSettings;									// настройки
	std::vector<sf::VideoMode> videoModes;									// видео режимы
	json j;

//------------------------------------------------------------------------------------------------------------------------
public:
	int music_volume = 0;
	int sound_volume = 0;
	int camera_speed = 1;
	float zoom_speed = 0;

	bool is_sound_play = false;
	bool is_music_play = false;

	std::string language;
};
//------------------------------------------------------------------------------------------------------------------------