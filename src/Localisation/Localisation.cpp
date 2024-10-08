#include "../stdafx.h"
#include "Localisation.hpp"

std::string Localization::getString(const std::string& key)
{
    if (localization.contains(key))
    {
        return localization.at(key);
    }
    else
    {
        LOG_WARN("{0} not found. Check localization file", key);
        return  "not found\t" + key;
    }
}

std::string Localization::getString(const std::string& icon, const std::string& key)
{
    if (localization.contains(key))
    {
        return icon + (std::string)localization.at(key);
    }
    else
    {
        LOG_WARN("{0} not found. Check localization file", key);
        return  "not found\t" + key;
    }
}

bool Localization::init()
{
    std::string filePath;

    if (WindowSettings::getInstance().language == "rus")
    {
        filePath = "translation/rus.json";
    }
    else if (WindowSettings::getInstance().language == "eng")
    {
        filePath = "translation/eng.json";
    }
    else
    {
        LOG_ERROR("Language doesn't supported");
        return false;
    }

    localization_file.open(filePath);

    if (!localization_file.is_open())
    {
        LOG_ERROR("The file was not found or could not be opened");
        return false;
    }

    localization_file >> localization;
    localization_file.close();
    return true;
}
