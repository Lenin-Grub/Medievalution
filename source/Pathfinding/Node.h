#pragma once
#include "../Core/Core.h"

/*
	nodeBase pos.x, pos.y;

	map[name, pos] nodes;

	addNode(node, pos);
	connectNode(connection);
	disconnectNode(connection);



	0000000000
	0*------00
	0000000|00
	0000000L*0
	0000000000
	
	1. установить node
	2. связать node с соседями
		2.1. уникальный цвет = 1 node
		2.2. найти "центр" провинции (в WorldMap)
		2.3. установить в "центр" node
		2.4. если node соседствует с другим node установить свзясь
		2.5. установить соседей node
	3. найти путь

*/

class Node
{
public:
	Node();
	virtual ~Node();

	void addNode(sf::Vector2i position);
	void removeNode();


	bool isVisited = false;
	//препятствие?
	bool isObstcale = false;

	sf::Vector2i position;

	std::vector <Node*> neighbours_nodes;
	Node* parent;

private:

};