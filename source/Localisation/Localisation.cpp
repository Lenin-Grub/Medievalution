#include "../stdafx.h"
#include "Localisation.hpp"


std::string Localisation::getStringByKey(std::string key)
{
	// to do сделать проверку на неправильный ключ или остутсвие строки
	// example
	// Localisation::getInstance().getStringByKey("key").c_str();
	return WindowSettings::getInstance().localisation.at(key);
}