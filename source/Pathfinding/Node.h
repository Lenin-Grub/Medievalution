#pragma once
#include "../Core/Core.h"

class Node
{
public:
	Node();
	virtual ~Node();

	void addNode(sf::Vector2f position);
	void removeNode();

	void setCost();
	void getCost();

	bool isVisited = false;

	//препятствие?
	bool isObstcale = false;
	int cost;

	sf::Vector2f position;

	std::vector <Node*> neighbours_nodes;
	Node* parent;

private:

};