#pragma once
#include "../Core/Core.h"

class Province
{
public:
	int id;
	std::string name;
	std::string comment;
	std::string owner;
	sf::Color color;

	void findNeighbors	(int porovince_id);
	void addNeighbor	(int porovince_id);
	void removeNeighbor	(int porovince_id);

	void setCentre(sf::Vector2i centre);
	void getCentre();

	void setOwner(std::string owner);
	void getOwner();
};