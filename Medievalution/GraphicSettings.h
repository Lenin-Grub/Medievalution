#pragma once
#include "System.h"

class GraphicSettings
{
public:
	GraphicSettings();

	std::string title;														// название программы
	sf::VideoMode resolution;												// расширение экрана
	bool fullscrean;														// полный экран ?
	bool vertycalSync;														// вертикальная синхронизация
	short unsigned fps_limit;												// лимит фпс
	sf::ContextSettings contextSettings;									// настройки
	std::vector<sf::VideoMode> videoModes;									// видео режимы

	void saveToFile(const std::string path);								// сохранине в файл
	void loadFromFIle(const std::string path);								// загрузка из файла
};