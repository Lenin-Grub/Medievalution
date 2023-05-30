﻿#pragma once
#include "../stdafx.h"
#include "../Input/Input.h"
//------------------------------------------------------------------------------------------------------------------------
class Core
{
public:
	static sf::Event sfmlEvent;						// события sfml
	static sf::View view;							// камера вида
	static sf::Clock clock;							// часыs
	static std::unique_ptr<sf::Music> music;							// музыка
	static float dtime;								// время
	//static Input input;								// нажатия

	static sf::Vector2i mousePosScreen;				// глобальное положение мыши на мониторе
	static sf::Vector2i mousePosWindow;				// положение мыши относительно окна приложения
	static sf::Vector2f mousePosView;				// положение мыши относительно камеры вида
	Core();

};
//------------------------------------------------------------------------------------------------------------------------
typedef Core core;
typedef sf::Vector2i v2i;					// v2i
typedef sf::Vector2f v2f;					// v2f
typedef sf::Vector2u v2u;					// v2u