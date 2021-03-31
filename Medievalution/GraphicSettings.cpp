#include "stdafx.h"
#include "GraphicSettings.h"
// îò
GraphicSettings::GraphicSettings()																// если graphic_settings.ini не обнаружено, то будут приняты эти настройки
{
	this->title = "Unitiolized";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscrean = false;
	this->vertycalSync = false;
	this->fps_limit = 120;
	this->contextSettings.antialiasingLevel = 0;
	this->videoModes = sf::VideoMode::getFullscreenModes();
}

void GraphicSettings::saveToFile(const std::string path)										// сохранение в /Config/graphic_settings.ini файл
{
	std::ofstream ofs(path);
	if (ofs.is_open())
	{
		ofs << this->title << std::endl;														// название программы
		ofs << this->resolution.width << " " << this->resolution.height << std::endl;			// расширение экрана. Первое значение ширина, второе-высота, между ними проебел, пример 1200 980
		ofs << this->fullscrean << std::endl;													// полный экран или оконный режим?
		ofs << this->fps_limit << std::endl;													// ограничение fps, по-умолчанию 120
		ofs << this->vertycalSync << std::endl;													// вертикальная синхронизация, по-умолчанию выкл
		ofs << this->contextSettings.antialiasingLevel << std::endl;							// антиалиасинг (сглаживание), по-умолчанию выкл
	}
	ofs.close();
}


void GraphicSettings::loadFromFIle(const std::string path)										// загрузка из файла /Config/graphic_settings.ini
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
	}
	ifs.close();
}