#pragma once
#include "../Core/Core.h"
#include "../Json/json.hpp"
#include "../Settings/WindowSettings.h"

/**
 * @class Localisation
 * @brief A singleton class for managing localization strings within an application.
 *
 * The Localisation class provides a centralized way to manage and retrieve localized strings
 * based on unique keys. It ensures that there is only one instance of the localization
 * data within the application.
 */

class Localisation
{
public:
 
/// @brief Gets the singleton instance of the Localisation class.
/// This function ensures that there is only one instance of the Localisation
/// class. It returns a reference to this single instance.
/// @return A reference to the single instance of the Localisation class.
	static Localisation& getInstance()
	{
		static Localisation single_instance;
		return single_instance;
	}

/// @brief Retrieves a localized string based on a unique key.
/// This function looks up a string associated with a given key and returns
/// the corresponding localized string.
/// @param key The unique key associated with the desired localized string.
/// @return The localized string corresponding to the given key.
	std::string getStringByKey(std::string key);

private:
	Localisation() {};
    Localisation(const Localisation& root)		   = delete;
    Localisation& operator=(const Localisation&) = delete;
private:
};