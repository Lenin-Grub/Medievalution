#include "../stdafx.h"
#include "Localisation.hpp"


std::string Localisation::getStringByKey(std::string key)
{
	// to do ������� �������� �� ������������ ���� ��� ��������� ������
	// example
	// Localisation::getInstance().getStringByKey("key").c_str();
	return WindowSettings::getInstance().localisation.at(key);
}