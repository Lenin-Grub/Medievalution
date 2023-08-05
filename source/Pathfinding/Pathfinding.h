#pragma once
#include "../Core/Core.h"
#include "Node.h"

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();

	void findPath();

private:

	std::vector <Node> nodes;

	sf::Vector2f startPos;
	sf::Vector2f endPos;
	
	bool isFind;
	bool noPath;
};