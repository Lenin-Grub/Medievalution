#pragma once
#include "../Core/Core.h"

struct Node
{
	sf::Vector2f position;  // ���������� ����
	int path_length;        // ���������� ����� ���� �� ���������� ���� �� �������� ����.
	int heurisic_cost;      // ���������� ������������� ������ ���������� �� �������� ���� �� ��������� ����.
	Node* parent;           // ��������� �� ��������

	Node(sf::Vector2f pos, int g, int h, Node* parent)
		: position(pos)
		, path_length(g)
		, heurisic_cost(h)
		, parent(parent)
	{
		//nothing
	}

	//��������� ������� ��������� ����������� ����� ������ ���� ��� ������ ���� � ��������� A*. 
	//��� ������ ���� ��� ��������� (������������ �������� �����) � ��������� A*.
	//���� � ����� ������ ��������� ������� findCost() ����� ����� ������� ���������.
	int findCost() const
	{
		return path_length + heurisic_cost;
	}
};

class Pathfinding
{
public:

	// ������� ��� ���������� �������������� ���������� ����� ������
	int heuristic(sf::Vector2f first, sf::Vector2f second)
	{
		return abs(first.x - second.x) + abs(first.y - second.y);
	}

	// ����� ����
	std::vector<sf::Vector2f> findPath(std::vector<std::vector<bool>>& grid, sf::Vector2f start, sf::Vector2f end)
	{
		int dx[4] = { 1, -1, 0, 0 };
		int dy[4] = { 0, 0, 1, -1 };

		int rows = grid.size();
		int cols = rows > 0 ? grid[0].size() : 0;

		std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> open([](Node* n1, Node* n2)
			{
				return n1->findCost() > n2->findCost();
			});

		std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

		Node* startNode = new Node(start, 0, heuristic(start, end), nullptr);
		open.push(startNode);

		while (!open.empty())
		{
			Node* current = open.top();
			open.pop();

			// �������� ���������� ������� �����
			if (current->position.x == end.x && current->position.y == end.y)
			{
				std::vector<sf::Vector2f> path;
				while (current != nullptr)
				{
					path.push_back({ current->position.x, current->position.y });
					current = current->parent;
				}
				reverse(path.begin(), path.end());

				return path;
			}

			visited[current->position.x][current->position.y] = true;

			for (int i = 0; i < 4; ++i) {
				int nextX = current->position.x + dx[i];
				int nextY = current->position.y + dy[i];

				if (nextX >= 0 && nextX < rows && nextY >= 0 && nextY < cols && !grid[nextX][nextY] && !visited[nextX][nextY])
				{
					Node* nextNode = new Node(sf::Vector2f(nextX, nextY), current->path_length + 1, heuristic(sf::Vector2f(nextX, nextY), end), current);
					open.push(nextNode);
				}
			}
		}

		return {}; // ���� �� ������
	}

	// �������� ����� ����
	void init()
	{
		std::vector<std::vector<bool>> grid =
		{
			{0, 0, 0, 0, 0},
			{0, 1, 1, 1, 1},
			{0, 0, 0, 0, 0},
			{1, 1, 0, 1, 1},
			{0, 0, 0, 0, 0}
		};

		sf::Vector2f start = { 0, 0 };
		sf::Vector2f end = { 4, 4 };

		std::vector<sf::Vector2f> path = findPath(grid, start, end);

		setlocale(LC_ALL, "rus");

		if (path.empty())
		{
			std::cout << "���� �� ������." << std::endl;
		}
		else
		{
			std::cout << "��������� ����:" << std::endl;
			for (auto& p : path)
			{
				std::cout << "(" << p.x << ", " << p.y << ") ";
			}
			std::cout << std::endl;
		}
	}
};