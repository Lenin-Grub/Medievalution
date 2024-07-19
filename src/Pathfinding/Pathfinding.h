#pragma once
#include "../Core/Core.h"
#include "Node.h"

class Pathfinding
{
public:
    Pathfinding();
    virtual ~Pathfinding() = default;

    void initNodes(int width, int height);
    void draw(sf::RenderWindow& window);
    void handleInput();
    void findPath(Node* start, Node* end);
    void resetNodes();
    int heuristic(Node* start, Node* end);

    Node* start_node;
    Node* end_node;
private:
    std::vector<std::vector<Node>> nodes;
    sf::Vector2f tile_size;
    int width;
    int height;
};