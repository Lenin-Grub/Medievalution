#pragma once
#include "../stdafx.h"
#include "../Input/Input.h"
#include "../Resource/Jukebox.hpp"
#include "../Math/Math.h"

class Core
{
public:
	static sf::Event sfml_event;
	static sf::View view;
	static sf::Clock clock;
	static float dtime;
	static sf::Vector2i mouse_pos_screen;
	static sf::Vector2i mouse_pos_window;
	static sf::Vector2f mouse_pos_view;
	static Math math;

	Core();
};

typedef Core core;
typedef sf::Vector2i vec2i;
typedef sf::Vector2f vec2f;
typedef sf::Vector2u vec2u;