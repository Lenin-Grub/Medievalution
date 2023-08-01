#include "../stdafx.h"
#include "Localisation.hpp"


std::string Localisation::getStringByKey(std::string key)
{
	//TODO ������� �������� �� ������������ ���� ��� ��������� ������
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