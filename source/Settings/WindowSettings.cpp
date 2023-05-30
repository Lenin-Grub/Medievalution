#include "../stdafx.h"
#include "WindowSettings.h"
//#include <../../../source/Json/json.hpp>
//------------------------------------------------------------------------------------------------------------------------
// если graphic_settings.ini не обнаружено, то будут приняты эти настройки
WindowSettings::WindowSettings()																
{
	this->title = "Unitiolized";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscrean = false;
	this->vertycalSync = false;
	this->fps_limit = 0;
	this->contextSettings.antialiasingLevel = 0;
	this->videoModes = sf::VideoMode::getFullscreenModes();
	this->music_volume = 100;
	this->camera_speed = 1;
	LOG_WARN("Window settings not found. Used default settings");
}

//------------------------------------------------------------------------------------------------------------------------
// сохранение в /Config/graphic_settings.ini файл
bool WindowSettings::saveToFile(const std::string path)
{

	json j{};
	j["title"] = title;
	j["resolution"] = resolution.width, resolution.height;
	j["fullscrean"] = fullscrean;
	j["fps_limit"] = fps_limit;
	j["vertycalSync"] = contextSettings.antialiasingLevel;
	j["music_volume"] = music_volume;
	j["camera_speed"] = camera_speed;
	auto s = j.dump();
	std::cout << s << std::endl;

	std::ofstream ofs(path);
	if (ofs.is_open())
	{
		ofs << this->title << std::endl;														// название программы
		ofs << this->resolution.width << " " << this->resolution.height << std::endl;			// расширение экрана. Первое значение ширина, второе-высота, между ними проебел, пример 1200 980
		ofs << this->fullscrean << std::endl;													// полный экран или оконный режим?
		ofs << this->fps_limit << std::endl;													// ограничение fps, по-умолчанию 120
		ofs << this->vertycalSync << std::endl;													// вертикальная синхронизация, по-умолчанию выкл
		ofs << this->contextSettings.antialiasingLevel << std::endl;							// антиалиасинг (сглаживание), по-умолчанию выкл
		ofs << this->music_volume << std::endl;
		ofs << this->camera_speed << std::endl;
		ofs.close();
		LOG_INFO ("Window settings saved successfully");
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------
// загрузка из файла /Config/graphic_settings.ini
bool WindowSettings::loadFromFIle(const std::string path)										
{
	std::ifstream ifs(path);

	if (ifs.is_open())
	{
		std::getline(ifs, this->title);															// название программы
		ifs >> this->resolution.width >> this->resolution.height;								// расширение экрана. Первое значение ширина, второе-высота, между ними проебел, пример 1200 980
		ifs >> this->fullscrean;																// полный экран или оконный режим?
		ifs >> this->fps_limit;																	// ограничение fps, по-умолчанию 120
		ifs >> this->vertycalSync;																// вертикальная синхронизация, по-умолчанию выкл
		ifs >> this->contextSettings.antialiasingLevel;											// антиалиасинг (сглаживание), по-умолчанию выкл
		ifs >> this->music_volume;
		ifs >> this->camera_speed;
		ifs.close();
		LOG_INFO("Window settings loaded successfully");
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------------------