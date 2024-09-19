#include "../stdafx.h"
#include "Input.h"

static const int    MAX_DOUBLE_CLICK_TIME = 200;
static unsigned     int last_click_time   = 0;
static sf::Vector2i last_click_position;

bool Input::isKeyPressed(const sf::Keyboard::Key& code)
{
	if (core::sfml_event.type == sf::Event::KeyPressed)
		if (core::sfml_event.key.code == code)
			return true;
	return false;
}

bool Input::isKeyReleased(const sf::Keyboard::Key& code)
{
	if (core::sfml_event.type == sf::Event::KeyReleased)
		if (core::sfml_event.key.code == code)
			return true;
	return false;
}

bool Input::isMousePressed(const sf::Mouse::Button& code)
{
	if (core::sfml_event.type == sf::Event::MouseButtonPressed)
		if (core::sfml_event.key.code == code)
			return true;
	return false;
}

bool Input::isMouseReleased(const sf::Mouse::Button& code)
{
	if (core::sfml_event.type == sf::Event::MouseButtonReleased)
		if (core::sfml_event.key.code == code)
			return true;
	return false;
}

bool Input::isMouseDoubleClick(const sf::Mouse::Button& code)
{
	if (isMouseReleased(code))
	{
		unsigned int currentTime = core::clock.getElapsedTime().asMilliseconds();

		if ((currentTime - last_click_time) < MAX_DOUBLE_CLICK_TIME && sf::Mouse::getPosition() == last_click_position)
		{
			last_click_time = 0;

			return true;
		}
		else 
		{
			last_click_time = currentTime;
			last_click_position = sf::Mouse::getPosition();
		}
	}
	return false;
}

bool Input::isMouseWheelUp(const sf::Mouse::Wheel code)
{
		if (core::sfml_event.mouseWheel.x == code)
			return true;
		return false;
}