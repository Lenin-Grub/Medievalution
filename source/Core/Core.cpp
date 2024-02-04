#include "../stdafx.h"
#include "Core.h"

class Input;

sf::Event				core::sfmlEvent;	
sf::View				core::view;			
sf::Clock				core::clock;
float					core::dtime;		

sf::Vector2i			core::mousePosScreen;
sf::Vector2i			core::mousePosWindow;
sf::Vector2f			core::mousePosView;	

Math					core::math;

Core::Core()
{
	setlocale(LC_ALL, "ru");
}