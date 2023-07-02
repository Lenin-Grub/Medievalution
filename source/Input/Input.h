#pragma once
//#include "stdafx.h"
#include "../Core/Core.h"

//------------------------------------------------------------------------------------------------------------------------
class Input
{
public:

	static bool isKeyPressed			(const sf::Keyboard::Key& code);		// была ли нажата кнопка мыши?

	static bool isKeyReleased			(const sf::Keyboard::Key& code);		// была ли отжата кнопка клавиатуры

	static bool isMousePressed			(const sf::Mouse::Button& code);		// была ли зажата кнопка мыши?

	static bool isMouseReleased			(const sf::Mouse::Button& code);		// была ли отжата кнопка мыши

	static bool isMouseWheelUp			(const sf::Mouse::Wheel code);			// было ли нажато(прокручено) колесико мышки?
};
//------------------------------------------------------------------------------------------------------------------------