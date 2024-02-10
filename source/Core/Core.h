#pragma once
#include "../stdafx.h"
#include "../Input/Input.h"
#include "../Resource/Jukebox.hpp"
#include "../Math/Math.h"
//------------------------------------------------------------------------------------------------------------------------
class Core
{
public:
	static sf::Event sfml_event;					// события sfml
	static sf::View view;							// камера вида
	static sf::Clock clock;							// часы
	static float dtime;								// время

	static sf::Vector2i mouse_pos_screen;			// глобальное положение мыши на мониторе
	static sf::Vector2i mouse_pos_window;			// положение мыши относительно окна приложения
	static sf::Vector2f mouse_pos_view;				// положение мыши относительно камеры вида
	static Math	math;

	Core();

    template <typename T>
    static void centerToLeft(sf::Transformable& object, T windowWidth);

    template <typename T>
    static void centerToRight(sf::Drawable& object, T windowWidth);

    template <typename T>
    static void centerToTop(sf::Drawable& object, T windowHeight);

    template <typename T>
    static void centerToBottom(sf::Transformable& object, T windowHeight);

    template <typename T>
    static void centerToTopLeft(sf::Drawable& object);

    template <typename T>
    static void centerToTopRight(sf::Drawable& object, T windowWidth);

    template <typename T>
    static void centerToBottomLeft(sf::Drawable& object, T windowHeight);

    template <typename T>
    static void centerToBottomRight(sf::Drawable& object, T windowWidth, T windowHeight);

    template <typename T>
    static void centerToCenter(sf::Drawable& object, T windowWidth, T windowHeight);
};

typedef Core core;
typedef sf::Vector2i vec2i;
typedef sf::Vector2f vec2f;
typedef sf::Vector2u vec2u;

template<typename T>
static void Core::centerToLeft(sf::Transformable& object, T windowWidth)
{
    sf::FloatRect bounds = object.getLocalBounds();
    object.setOrigin(0, bounds.height / 2);
    object.setPosition(0, windowWidth / 2);
}

template<typename T>
static void Core::centerToRight(sf::Drawable& object, T windowWidth)
{
    sf::FloatRect bounds = object.getLocalBounds();
    object.setOrigin(bounds.width, bounds.height / 2);
    object.setPosition(windowWidth, windowWidth / 2);
}

template<typename T>
static void Core::centerToTop(sf::Drawable& object, T windowHeight)
{
    sf::FloatRect bounds = object.getLocalBounds();
    object.setOrigin(bounds.width / 2, 0);
    object.setPosition(windowHeight / 2, 0);
}

template<typename T>
static void Core::centerToBottom(sf::Transformable& object, T windowHeight)
{
//    sf::FloatRect bounds = object.getLocalBounds();
//    object.setOrigin(bounds.width / 2, bounds.height);
    object.setPosition(windowHeight / 2, windowHeight);
}

template<typename T>
static void Core::centerToTopLeft(sf::Drawable& object)
{
    sf::FloatRect bounds = object.getLocalBounds();
    object.setOrigin(0, 0);
}

template<typename T>
static void Core::centerToTopRight(sf::Drawable& object, T windowWidth)
{
    sf::FloatRect bounds = object.getLocalBounds();
    object.setOrigin(bounds.width, 0);
    object.setPosition(windowWidth, 0);
}

template<typename T>
static void Core::centerToBottomLeft(sf::Drawable& object, T windowHeight)
{
    sf::FloatRect bounds = object.getLocalBounds();
    object.setOrigin(0, bounds.height);
    object.setPosition(0, windowHeight);
}

template<typename T>
static void Core::centerToBottomRight(sf::Drawable& object, T windowWidth, T windowHeight)
{
    sf::FloatRect bounds = object.getLocalBounds();
    object.setOrigin(bounds.width, bounds.height);
    object.setPosition(windowWidth, windowHeight);
}

template<typename T>
static void Core::centerToCenter(sf::Drawable& object, T windowWidth, T windowHeight)
{
    sf::FloatRect bounds = object.getLocalBounds();
    object.setOrigin(bounds.width / 2, bounds.height / 2);
    object.setPosition(windowWidth / 2, windowHeight / 2);
}