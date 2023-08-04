#pragma once
#include "../Core/Core.h"

class Province
{
public:
	int id;
	std::string name;
	std::string comment;
	sf::Color color;

	std::string owner;

	void findNeighbors(int porovince_id);
	void addNeighbor(int porovince_id);
	void removeNeighbor(int porovince_id);

	void setCentre(sf::Vector2i centre);
	void getCentre();
};