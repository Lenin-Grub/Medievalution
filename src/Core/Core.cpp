#include "../stdafx.h"
#include "Core.h"

class Input;

sf::Event				core::sfml_event;	
sf::View				core::view;			
sf::Clock				core::clock;
float					core::dtime;		

sf::Vector2i			core::mouse_pos_screen;
sf::Vector2i			core::mouse_pos_window;
sf::Vector2f			core::mouse_pos_view;

Math					core::math;

Core::Core()
{
	system("chcp 1251");
	setlocale(LC_ALL, "ru");
}