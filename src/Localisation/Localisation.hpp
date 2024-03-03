#pragma once
#include "../Core/Core.h"
#include "../Json/json.hpp"
#include "../Settings/WindowSettings.h"

class Localisation
{
public:
	static Localisation& getInstance()
	{
		static Localisation single_instance;
		return single_instance;
	}

	std::string getStringByKey(std::string key);

private:
    Localisation() {}
    Localisation(const Localisation& root) = delete;
    Localisation& operator=(const Localisation&) = delete;
private:
};