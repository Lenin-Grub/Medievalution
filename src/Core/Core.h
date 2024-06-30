#pragma once
#include "../stdafx.h"
#include "../Input/Input.h"
#include "../Resource/Jukebox.hpp"
#include "../Math/Math.h"
//------------------------------------------------------------------------------------------------------------------------
class Core
{
public:
	static sf::Event sfml_event;            // события sfml
	static sf::View view;                   // камера вида
	static sf::Clock clock;                 // часы
	static float dtime;                     // время
	static sf::Vector2i mouse_pos_screen;   // глобальное положение мыши на мониторе
	static sf::Vector2i mouse_pos_window;   // положение мыши относительно окна приложения
	static sf::Vector2f mouse_pos_view;     // положение мыши относительно камеры вида
	static Math math;

	Core();
};

typedef Core core;
typedef sf::Vector2i vec2i;
typedef sf::Vector2f vec2f;
typedef sf::Vector2u vec2u;