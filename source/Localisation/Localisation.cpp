#include "../stdafx.h"
#include "Localisation.hpp"


std::string Localisation::getStringByKey(std::string key)
{
	//TODO сделать проверку на неправильный ключ или остутсвие строки
	if (!WindowSettings::getInstance().localisation.at(key).empty())
	{
		return WindowSettings::getInstance().localisation.at(key);
	}
	else
	{
		LOG_WARN("NOT FOUND ", key, ". Check localisation file");
	}
	return "not_found_key";
}