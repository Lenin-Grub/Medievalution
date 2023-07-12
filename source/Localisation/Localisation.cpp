#include "../stdafx.h"
#include "Localisation.hpp"


std::string Localisation::getStringByKey(std::string key)
{
	// to do ������� �������� �� ������������ ���� ��� ��������� ������
	// example
	// Localisation::getInstance().getStringByKey("key").c_str();
	if (!WindowSettings::getInstance().localisation.at(key).empty())
	{
		return WindowSettings::getInstance().localisation.at(key);
	}
	return "not found key";
}