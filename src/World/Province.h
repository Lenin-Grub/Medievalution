#pragma once
#include "../Core/Core.h"

struct Province
{
	int          id = -1;
	std::string  name;
	std::string  comment;
	std::string  owner;
	sf::Vector2f centre;
	sf::Color    color;
};