#include "../stdafx.h"
#include "Localisation.hpp"

#pragma region Json
LocalizationJson::LocalizationJson(const nlohmann::json& localization)
    : localization(localization)
{
}

bool LocalizationJson::contains(const std::string& key) const
{
    return localization.contains(key);
}

std::string LocalizationJson::get(const std::string& key) const
{
    return localization.at(key);
}
#pragma endregion

#pragma region String
Localization::Localization()
    : storage(nullptr)
{
}

LocalizationString::LocalizationString(const std::string& localization)
    : localization(localization)
{
}

bool LocalizationString::contains(const std::string& key) const
{
    // Implementation for std::string
    // For example, you can use std::map<std::string, std::string>
    // or std::unordered_map<std::string, std::string>
    return localization.find(key) != std::string::npos;
}

std::string LocalizationString::get(const std::string& key) const
{
    size_t pos = localization.find(key);
    if (pos != std::string::npos) 
    {
        size_t endPos = localization.find('\n', pos);
        if (endPos == std::string::npos) 
            endPos = localization.length();
        return localization.substr(pos + key.length() + 1, endPos - pos - key.length() - 1);
    }
    return "";
}
#pragma endregion

Localization& Localization::getInstance()
{
    static Localization single_instance;
    return single_instance;
}

std::string Localization::get(const std::string& key)
{
    if (storage->contains(key)) 
        return storage->get(key);
    else 
    {
        LOG_WARN("{0} not found. Check localization file", key);
        return "not found\t" + key;
    }
}

std::string Localization::get(const std::string& icon, const std::string& key)
{
    if (storage->contains(key)) 
        return icon + storage->get(key);
    else 
    {
        LOG_WARN("{0} not found. Check localization file", key);
        return "not found\t" + key;
    }
}

bool Localization::init(const std::string& filePath)
{
    std::ifstream localization_file(filePath);
    if (!localization_file.is_open()) 
    {
        LOG_ERROR("The localization file was not found or could not be opened. Incorrect path: \"{}\"", filePath);
        return false;
    }
    nlohmann::json localization;
    localization_file >> localization;
    localization_file.close();
    storage = std::make_unique<LocalizationJson>(localization);

    // or i can not use json if I whant chage it in future
    // std::string localization_string((std::istreambuf_iterator<char>(localization_file)), std::istreambuf_iterator<char>());
    // storage_ = std::make_unique<StringLocalizationStorage>(localization_string);
    return true;
}