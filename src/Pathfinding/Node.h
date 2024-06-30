#pragma once
#include "../Core/Core.h"

struct Node
{
	sf::Vector2f position;  // координаты узла
	int          path_length;        // обозначает длину пути от начального узла до текущего узла.
	int          heurisic_cost;      // обозначает эвристическую оценку расстояния от текущего узла до конечного узла.
	Node*        parent;           // указатель на родителя

	Node(sf::Vector2f pos, int path_length, int heurisic_cost, Node* parent)
		: position(pos)
		, path_length(path_length)
		, heurisic_cost(heurisic_cost)
		, parent(parent)
	{
		//nothing
	}

	//Позволяет оценить стоимость прохождения через данный узел при выборе пути в алгоритме A*. 
	//При выборе узла для раскрытия (рассмотрения соседних узлов) в алгоритме A*.
	//Узлы с более низким значением функции findCost() имеют более высокий приоритет.
	int findCost() const
	{
		return path_length + heurisic_cost;
	}
};

class Pathfinding
{
public:

	// Функция для вычисления эвристического расстояния между узлами
	int heuristic(sf::Vector2f first, sf::Vector2f second)
	{
		return abs(first.x - second.x) + abs(first.y - second.y);
	}

	// Поиск пути
	std::vector<sf::Vector2f> findPath(std::vector<std::vector<bool>>& grid, sf::Vector2f start, sf::Vector2f end)
	{
		/*
		East, West,
		North, South ,
		South-East, South-West,
		North-East, North-West
		*/
		const int DIRS = 8;

		std::array<sf::Vector2f, DIRS> dirs =
		{
			sf::Vector2f {  1,  0 },	sf::Vector2f { -1,  0 },
			sf::Vector2f {  0, -1 },	sf::Vector2f {  0,  1 },
			sf::Vector2f {  1,  1 },	sf::Vector2f {  1, -1 },
			sf::Vector2f { -1,  1 },	sf::Vector2f { -1, -1 }
		};

		#pragma region field
		int rows = grid.size();
		int cols = rows > 0 ? grid[0].size() : 0;

		std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> unvisited
		([](Node* n1, Node* n2)
			{
				return n1->findCost() > n2->findCost();
			}
		);

		std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

		Node* startNode = new Node(start, 0, heuristic(start, end), nullptr);
		unvisited.push(startNode);
		#pragma endregion

		while (!unvisited.empty())
		{
			Node* current = unvisited.top();
			unvisited.pop();

			// Проверка достижения целевой точки
			if (current->position == end)
			{
				std::vector<sf::Vector2f> path;
				while (current != nullptr)
				{
					path.push_back(current->position);
					current = current->parent;
				}
				reverse(path.begin(), path.end());

				return path;
			}

			visited[current->position.x][current->position.y] = true;

			for (int i = 0; i < DIRS; ++i)
			{
				sf::Vector2f next = current->position + dirs[i];

				if (next.x >= 0 && next.x < rows && 
					next.y >= 0 && next.y < cols && 
					!grid[next.x][next.y] && 
					!visited[next.x][next.y])
				{
					Node* nextNode = new Node(sf::Vector2f(next), current->path_length + 1, heuristic(sf::Vector2f(next), end), current);
					unvisited.push(nextNode);
				}
			}
		}
		return {}; // путь не найден
	}

	// Тестовый поиск пути
	void init()
	{
		std::vector<std::vector<bool>> grid 
		{
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 0, 1, 0},
			{0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 0, 1, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
			{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
		};

		sf::Vector2f start = { 0, 0 };
		sf::Vector2f end = { 4, 4 };

		std::vector<sf::Vector2f> path = findPath(grid, start, end);

		setlocale(LC_ALL, "rus");

		if (path.empty())
		{
			std::cout << "Путь не найден." << std::endl;
		}
		else
		{
			std::cout << "Найденный путь:" << std::endl;
			for (auto& p : path)
			{
				std::cout << "(" << p.x << ", " << p.y << ") ";
			}
			std::cout << std::endl;
		}
	}
};