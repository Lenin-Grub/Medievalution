#pragma once
#include "../Core/Core.h"

class Input
{
public:
	static bool isKeyPressed		(const sf::Keyboard::Key& code);
	static bool isKeyReleased		(const sf::Keyboard::Key& code);

	static bool isMousePressed		(const sf::Mouse::Button& code);
	static bool isMouseReleased		(const sf::Mouse::Button& code);
	static bool isMouseDoubleClick	(const sf::Mouse::Button& code);

	static bool isMouseWheelUp		(const sf::Mouse::Wheel code);
};