#pragma once
#include "Common/Common.h"
#include "Json/json.hpp"
#include "Settings/WindowSettings.h"

/// @brief Interface for localization.
class ILocalization
{
public:
    virtual ~ILocalization() = default;

    /// @brief Checks if the localization contains a key
    /// @param key The key to check.
    /// @return true if the key is contained, false otherwise.
    virtual bool contains(const std::string& key) const = 0;

    /// @brief Returns the value for a key.
    /// @param key The key to retrieve the value for.
    /// @return The value for the key.
    virtual std::string get(const std::string& key) const = 0;
};

/// @brief JSON-based localization implementation.
class LocalizationJson 
    : public ILocalization 
{
public:
    LocalizationJson(const nlohmann::json& localization);

    bool contains(const std::string& key)   const override;
    std::string get(const std::string& key) const override;

private:
    nlohmann::json localization;
};

/// @brief String-based localization implementation.
class LocalizationString 
    : public ILocalization 
{
public:
    LocalizationString(const std::string& localization);

    bool contains(const std::string& key)   const override;
    std::string get(const std::string& key) const override;

private:
    std::string localization;
};

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
    static Localization& getInstance();

    /// @brief Retrieves a localized string based on a unique key.
    /// This function looks up a string associated with a given key and returns
    /// the corresponding localized string.
    /// @param key The unique key associated with the desired localized string.
    /// @return The localized string corresponding to the given key.
    std::string get(const std::string& key);

    /// @brief Retrieves a localized string based on a unique key.
    /// This function looks up a string associated with a given key and returns
    /// the corresponding localized string.
    /// @param icon The icon is based on a font that is inserted before string
    /// @param key The unique key associated with the desired localized string.
    /// @return The localized string corresponding to the given key.
    std::string get(const std::string& icon, const std::string& key);

    /// @brief Init localisation file
    bool init(const std::string& filePath);

private:
    Localization();
    Localization(const Localization& root)       = delete;
    Localization& operator=(const Localization&) = delete;

private:
    std::ifstream  localization_file;
    std::unique_ptr<ILocalization> storage;
};