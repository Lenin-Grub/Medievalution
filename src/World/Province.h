#pragma once
#include "../Core/Core.h"

struct Province
{
	int          id;	  ///< @brief The unique identifier for the province.
	std::string  name;    ///< @brief The name of the province.
	std::string  comment; ///< @brief A comment or description about the province. Just for development, not used in game
	std::string  owner;   ///< @brief The owner of the province.
	sf::Vector2f centre;  ///< @brief The geographical center of the province.
	sf::Color    color;   ///< @brief The color associated with the province.
};