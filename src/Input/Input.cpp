#include "../stdafx.h"
#include "Input.h"

// нажата ли клавиша
bool Input::isKeyPressed(const sf::Keyboard::Key& code)
{
	if (core::sfml_event.type == sf::Event::KeyPressed)
		if (core::sfml_event.key.code == code)
			return true;
	return false;
}

// отпущена ли клавиша
bool Input::isKeyReleased(const sf::Keyboard::Key& code)
{
	if (core::sfml_event.type == sf::Event::KeyReleased)
		if (core::sfml_event.key.code == code) 
			return true;
	return false;
}

// нажата ли мышка
bool Input::isMousePressed(const sf::Mouse::Button& code)
{
		if (core::sfml_event.type == sf::Event::MouseButtonPressed)
			if (core::sfml_event.key.code == code) 
				return true;
		return false;
}

// отпущена ли мышка
bool Input::isMouseReleased(const sf::Mouse::Button& code)
{
		if (core::sfml_event.type == sf::Event::MouseButtonReleased)
			if (core::sfml_event.key.code == code) 
				return true;
		return false;
}

// прокручено ли колесико мышки
bool Input::isMouseWheelUp(const sf::Mouse::Wheel code)
{
		if (core::sfml_event.mouseWheel.x == code)
			return true;
		return false;
}