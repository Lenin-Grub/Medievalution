#include "../stdafx.h"
#include "WindowSettings.h"
//#include <../../../source/Json/json.hpp>
//------------------------------------------------------------------------------------------------------------------------
// если graphic_settings.ini не обнаружено, то будут приняты эти настройки
WindowSettings::WindowSettings()																
{
	title = "Unitiolized";
	resolution = sf::VideoMode::getDesktopMode();
	fullscrean = false;
	vertycalSync = false;
	fps_limit = 0;
	contextSettings.antialiasingLevel = 0;
	videoModes = sf::VideoMode::getFullscreenModes();
	music_volume = 100;
	camera_speed = 1;
	zoom_speed = 0;
	language = "rus";
	LOG_WARN("Window settings not found. Used default settings");
}

//------------------------------------------------------------------------------------------------------------------------
// сохранение в /Config/graphic_settings.ini файл
bool WindowSettings::saveToFile(const std::string path)
{
	nlohmann::ordered_json j;
	std::ofstream file("config/settings.json");
	std::ofstream ofs(path);
	if (ofs.is_open())
	{
		//to do json ругается на синхронизацию, т.к. она int, а требуется bool
		//j["vertycalSync"] = contextSettings.antialiasingLevel;
		j["title"] = title;
		j["resolution"] = { {"x" , resolution.width}, {"y",resolution.height} };
		j["fullscrean"] = fullscrean;
		j["fps_limit"] = fps_limit;
		j["music_volume"] = music_volume;
		j["camera_speed"] = camera_speed;
		j["zoom_speed"] = zoom_speed;
		j["language"] = language;
		file << std::setw(4) << j;
		ofs.close();
		LOG_INFO("Window settings saved successfully");
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------
// загрузка из файла /Config/graphic_settings.ini
bool WindowSettings::loadFromFIle(const std::string path)										
{
	nlohmann::json j;
	std::ifstream ifs(path);
	j = nlohmann::json::parse(ifs);
	

	if (ifs.is_open())
	{
		//to do json ругается на синхронизацию, т.к. она int, а требуется bool
		//j["vertycalSync"].get_to(vertycalSync);
		j["title"].get_to(title);
		j["resolution"]["x"].get_to(resolution.width);
		j["resolution"]["y"].get_to(resolution.height);
		j["fullscrean"].get_to(fullscrean);
		j["fps_limit"].get_to(fps_limit);
		j["music_volume"].get_to(music_volume);
		j["camera_speed"].get_to(camera_speed);
		j["zoom_speed"].get_to(zoom_speed);
		j["language"].get_to(language);
		ifs.close();
		LOG_INFO("Window settings loaded successfully");
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------------------