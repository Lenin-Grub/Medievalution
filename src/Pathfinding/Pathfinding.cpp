#include "../stdafx.h"
#include "Pathfinding.h"

Pathfinding::Pathfinding()
    : width(10)
    , height(10)
    , start_node(nullptr)
    , end_node(nullptr)
    , tile_size(sf::Vector2f(10, 10))
{
}

void Pathfinding::initNodes(int width, int height)
{
    this->width = width;
    this->height = height;

    nodes.resize(width, std::vector<Node>(height));

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            nodes[x][y].position = sf::Vector2f(x, y);
            nodes[x][y].walkable = true;
            nodes[x][y].is_visited = false;
            nodes[x][y].parent = nullptr;
            nodes[x][y].gCost = 0;

            if (x > 0)
                nodes[x][y].neighbors.push_back(&nodes[x - 1][y]);
            if (x < width - 1)
                nodes[x][y].neighbors.push_back(&nodes[x + 1][y]);
            if (y > 0)
                nodes[x][y].neighbors.push_back(&nodes[x][y - 1]);
            if (y < height - 1)
                nodes[x][y].neighbors.push_back(&nodes[x][y + 1]);

            // диагональ
            if (x > 0 && y > 0)
                nodes[x][y].neighbors.push_back(&nodes[x - 1][y - 1]);
            if (x > 0 && y < height - 1)
                nodes[x][y].neighbors.push_back(&nodes[x - 1][y + 1]);
            if (x < width - 1 && y > 0)
                nodes[x][y].neighbors.push_back(&nodes[x + 1][y - 1]);
            if (x < width - 1 && y < height - 1)
                nodes[x][y].neighbors.push_back(&nodes[x + 1][y + 1]);
        }
    }
}

void Pathfinding::draw(sf::RenderWindow& window)
{
    sf::RectangleShape rect(tile_size);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (!nodes[x][y].walkable)
                rect.setFillColor(sf::Color::Black);
            else if (&nodes[x][y] == start_node)
                rect.setFillColor(sf::Color::Green);
            else if (&nodes[x][y] == end_node)
                rect.setFillColor(sf::Color::Red);
            else
            {
                rect.setFillColor(sf::Color::White);
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(1);
            }

            rect.setPosition(x * tile_size.x, y * tile_size.y);
            window.draw(rect);
        }
    }

    if (end_node != nullptr)
    {
        Node* p = end_node;
        while (p->parent != nullptr)
        {
            rect.setFillColor(sf::Color::Blue);
            rect.setPosition(p->position.x * tile_size.x, p->position.y * tile_size.y);
            window.draw(rect);
            p = p->parent;
        }
    }

}

void Pathfinding::handleInput()
{
    int x = core::mouse_pos_view.x / tile_size.x;
    int y = core::mouse_pos_view.y / tile_size.y;

    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            nodes[x][y].walkable = false;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            nodes[x][y].walkable = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            start_node = &nodes[x][y];
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            end_node = &nodes[x][y];
        }
    }
}

void Pathfinding::findPath(Node* start, Node* end)
{
    resetNodes();
    start_node = start;
    end_node = end;

    std::priority_queue<Node*, std::vector<Node*>, Node> openList;
    std::vector<Node*> closedList;

    if (start_node == nullptr || end_node == nullptr)
    {
        return;
    }

    if (start_node == end_node)
    {
        return;
    }

    start_node->gCost = 0.0f;
    start_node->hCost = heuristic(start_node, end_node);
    start_node->fCost = start_node->gCost + start_node->hCost;

    openList.push(start_node);

    while (!openList.empty())
    {
        Node* current = openList.top();
        openList.pop();

        closedList.push_back(current);

        if (current == end_node)
        {
            return;
        }

        for (Node* neighbor : current->neighbors)
        {
            if (neighbor->walkable && !neighbor->is_visited)
            {
                float tentativeGCost = current->gCost + 1;
                if (tentativeGCost < neighbor->gCost)
                {
                    neighbor->gCost = tentativeGCost;
                    neighbor->hCost = heuristic(neighbor, end_node);
                    neighbor->fCost = neighbor->gCost + neighbor->hCost;
                    neighbor->parent = current;
                    openList.push(neighbor);
                    neighbor->is_visited = true;
                }
            }
        }
    }
}

void Pathfinding::resetNodes()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            nodes[x][y].is_visited = false;
            nodes[x][y].gCost = INFINITY;
            nodes[x][y].fCost = INFINITY;
            nodes[x][y].parent = nullptr;
        }
    }
}

int Pathfinding::heuristic(Node* start, Node* end)
{
    return std::abs(start->position.x - end->position.x) + std::abs(start->position.y - end->position.y);
}
