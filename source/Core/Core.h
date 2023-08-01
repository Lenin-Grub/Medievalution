#pragma once
#include "../stdafx.h"
#include "../Input/Input.h"
//------------------------------------------------------------------------------------------------------------------------
class Core
{
public:
	static sf::Event sfmlEvent;						// события sfml
	static sf::View view;							// камера вида
	static sf::Clock clock;							// часы
	static std::unique_ptr<sf::Music> music;		// музыка
	static float dtime;								// время

	static sf::Vector2i mousePosScreen;				// глобальное положение мыши на мониторе
	static sf::Vector2i mousePosWindow;				// положение мыши относительно окна приложения
	static sf::Vector2f mousePosView;				// положение мыши относительно камеры вида
	Core();

};

typedef Core core;
typedef sf::Vector2i vec2i;					
typedef sf::Vector2f vec2f;					
typedef sf::Vector2u vec2u;					