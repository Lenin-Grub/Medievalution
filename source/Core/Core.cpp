﻿#include "../stdafx.h"
#include "Core.h"

class Input;

//------------------------------------------------------------------------------------------------------------------------
sf::Event				core::sfmlEvent;					// события sfml
sf::View				core::view;							// камера вида
sf::Clock				core::clock;
std::unique_ptr<sf::Music> core::music;						// музыка
float					core::dtime;						// время

//Input					S::input;							// нажатия игрока

sf::Vector2i			core::mousePosScreen;				// глобальное положение мыши на мониторе
sf::Vector2i			core::mousePosWindow;				// положение мыши относительно окна приложения
sf::Vector2f			core::mousePosView;					// положение мыши относительно камеры вида

//------------------------------------------------------------------------------------------------------------------------
Core::Core()
{
	setlocale(LC_ALL, "ru");
}
//------------------------------------------------------------------------------------------------------------------------