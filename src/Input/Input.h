#pragma once
#include "../Core/Core.h"

/**
 * @class Input
 * @brief A static class for handling input events in an application.
 *
 * The Input class provides static methods to check the status of keyboard and mouse events.
 * These methods can be used to determine whether specific keys or mouse buttons are pressed,
 * released, or double-clicked, as well as checking for mouse wheel movement.
 */
class Input
{
public:

/// @brief Checks if a specific key is currently pressed
/// @param code The key code to check.
/// @return True if the key is pressed, false otherwise.
	static bool isKeyPressed			(const sf::Keyboard::Key& code);

/// @brief Checks if a specific key was just released
/// @param code The key code to check.
/// @return True if the key was released, false otherwise.
	static bool isKeyReleased			(const sf::Keyboard::Key& code);

/// @brief Checks if a specific mouse button is currently pressed
///  @param code The mouse button code to check.
///  @return True if the mouse button is pressed, false otherwise.
	static bool isMousePressed			(const sf::Mouse::Button& code);

/// @brief Checks if a specific mouse button was just released
/// @param code The mouse button code to check.
/// @return True if the mouse button was released, false otherwise.
	static bool isMouseReleased			(const sf::Mouse::Button& code);

/// @brief Checks if a specific mouse button was double - clicked
/// @param code The mouse button code to check.
/// @return True if the mouse button was double - clicked, false otherwise.
	static bool isMouseDoubleClick		(const sf::Mouse::Button& code);

/// @brief Checks if the mouse wheel was scrolled up
/// @param code The mouse wheel code to check.
/// @return True if the mouse wheel was scrolled up, false otherwise.
	static bool isMouseWheelUp			(const sf::Mouse::Wheel code);
};