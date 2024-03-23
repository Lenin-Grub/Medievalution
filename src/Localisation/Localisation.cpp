#include "../stdafx.h"
#include "Localisation.hpp"

std::string Localisation::getStringByKey(std::string key)
{
	if (WindowSettings::getInstance().localisation.contains(key))
	{
		return WindowSettings::getInstance().localisation.at(key);
	}
	else
	{
		LOG_WARN("{0} not found. Check localisation file", key);
		return  "not found\t" + key;
	}
}