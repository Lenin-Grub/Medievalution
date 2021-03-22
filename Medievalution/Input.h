#pragma once
#include "Define.h"

class System;

class Input
{
public:

	virtual bool isKeyPressed			(const sf::Keyboard::Key& code);												// была ли нажата кнопка мыши?

	virtual bool isKeyReleased			(const sf::Keyboard::Key& code);												// была ли отжата кнопка клавиатуры

	virtual bool isMousePressed			(const sf::Mouse::Button& code);												// была ли зажата кнопка мыши?

	virtual bool isMouseReleased		(const sf::Mouse::Button& code);												// была ли отжата кнопка мыши

	virtual bool isMouseWheelUp			(const sf::Mouse::Wheel code);													// было ли нажато(прокручено) колесико мышки?

	virtual bool isMouseDoubleReleased	(const sf::Mouse::Button& code);												// двойной клик
};

