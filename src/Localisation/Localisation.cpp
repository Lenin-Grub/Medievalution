#include "../stdafx.h"
#include "Localisation.hpp"

std::string Localization::get(const std::string& key)
{
    if (localization.contains(key))
        return getLocalizationByKey(key);
    else
    {
        LOG_WARN("{0} not found. Check localization file", key);
        return  "not found\t" + key;
    }
}

std::string Localization::get(const std::string& icon, const std::string& key)
{
    if (localization.contains(key))
        return icon + getLocalizationByKey(key);
    else
    {
        LOG_WARN("{0} not found. Check localization file", key);
        return  "not found\t" + key;
    }
}

bool Localization::init()
{
    std::string filePath = "translation/rus.json";

    //TODO bad realization
    //if (WindowSettings::getInstance().language == "rus")
    //{
    //    filePath = "translation/rus.jso
    // n";
    //}
    //else if (WindowSettings::getInstance().language == "eng")
    //{
    //    filePath = "translation/eng.json";
    //}
    //else
    //{
    //    LOG_ERROR("Language doesn't supported");
    //    return false;
    //}

    localization_file.open(filePath);

    if (!localization_file.is_open())
    {
        LOG_ERROR("The localization file was not found or could not be opened. Incorrect path: \"{}\"", filePath);
        return false;
    }

    localization_file >> localization;

    localization_file.close();
    return true;
}

const std::string Localization::getLocalizationByKey(const std::string& key)
{
    return localization.at(key);
}
