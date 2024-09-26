#include "../stdafx.h"
#include "Localisation.hpp"

std::string Localization::getStringByKey(std::string key)
{
	if (WindowSettings::getInstance().localization.contains(key))
	{
		return WindowSettings::getInstance().localization.at(key);
	}
	else
	{
		LOG_WARN("{0} not found. Check localization file", key);
		return  "not found\t" + key;
	}
}