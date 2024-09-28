#pragma once
#include "../Core/Core.h"
#include "../Json/json.hpp"
#include "../Settings/WindowSettings.h"

/**
 * @class Localization
 * @brief A singleton class for managing localization strings within an application.
 *
 * The Localization class provides a centralized way to manage and retrieve localized strings
 * based on unique keys. It ensures that there is only one instance of the localization
 * data within the application.
 */

class Localization
{
public:
 
/// @brief Gets the singleton instance of the Localization class.
/// This function ensures that there is only one instance of the Localization
/// class. It returns a reference to this single instance.
/// @return A reference to the single instance of the Localization class.
    static Localization& getInstance()
    {
        static Localization single_instance;
        return single_instance;
    }

/// @brief Retrieves a localized string based on a unique key.
/// This function looks up a string associated with a given key and returns
/// the corresponding localized string.
/// @param key The unique key associated with the desired localized string.
/// @return The localized string corresponding to the given key.
    std::string getStringByKey(std::string key);

/// @brief Init loaclisation file
    bool init();

private:
    Localization()                               = default;
    Localization(const Localization& root)       = delete;
    Localization& operator=(const Localization&) = delete;
private:
    std::ifstream      localization_file;
};