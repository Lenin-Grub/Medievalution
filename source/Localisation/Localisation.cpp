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
		LOG_WARN("KEY NOT FOUND  Check localisation file");
		return  "not_found_key";
	}
}