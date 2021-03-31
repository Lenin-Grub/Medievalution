#include "stdafx.h"
#include "Input.h"
#include "System.h"
// îò
bool Input::isKeyPressed(const sf::Keyboard::Key& code)
{
	if (S::sfmlEvent.type == sf::Event::KeyPressed)
		if (S::sfmlEvent.key.code == code) return true;
	return false;
}

bool Input::isKeyReleased(const sf::Keyboard::Key& code)
{
		if (S::sfmlEvent.type == sf::Event::KeyReleased)
			if (S::sfmlEvent.key.code == code) return true;
		return false;
}

bool Input::isMousePressed(const sf::Mouse::Button& code)
{
		if (S::sfmlEvent.type == sf::Event::MouseButtonPressed)
			if (S::sfmlEvent.key.code == code) return true;
		return false;
}

bool Input::isMouseReleased(const sf::Mouse::Button& code)
{
		if (S::sfmlEvent.type == sf::Event::MouseButtonReleased)
			if (S::sfmlEvent.key.code == code) return true;
		return false;
}

bool Input::isMouseWheelUp(const sf::Mouse::Wheel code)
{
		if (S::sfmlEvent.mouseWheel.x == code) return true;
		return false;
}

bool Input::isMouseDoubleReleased(const sf::Mouse::Button& code)
{
	float timer;
	sf::Clock clock;

		if (S::sfmlEvent.type == sf::Event::MouseButtonReleased && S::sfmlEvent.key.code == code)
		{
			timer = clock.getElapsedTime().asMilliseconds();
			if (S::sfmlEvent.type == sf::Event::MouseButtonReleased && S::sfmlEvent.key.code == code && timer <= 300.0f)
			{
				cout << "click!\n";
				return true;
			}
			else {
				cout << "ooops!\n";
				clock.restart();
				return false;
			}
		}
}
