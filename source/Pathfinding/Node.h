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
	
	1. ���������� node
	2. ������� node � ��������
		2.1. ���������� ���� = 1 node
		2.2. ����� "�����" ��������� (� WorldMap)
		2.3. ���������� � "�����" node
		2.4. ���� node ����������� � ������ node ���������� ������
		2.5. ���������� ������� node
	3. ����� ����

*/

class Node
{
public:
	Node();
	virtual ~Node();

	void addNode(sf::Vector2i position);
	void removeNode();


	bool isVisited = false;
	//�����������?
	bool isObstcale = false;

	sf::Vector2i position;

	std::vector <Node*> neighbours_nodes;
	Node* parent;

private:

};