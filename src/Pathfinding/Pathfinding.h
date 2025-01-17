#pragma once
#include "Common/Common.h"
#include "Node.h"

// Hash function for sf::Vector2f
struct Vector2fHash 
{
    std::size_t operator()(const sf::Vector2f& v) const {
        std::size_t h1 = std::hash<float>()(v.x);
        std::size_t h2 = std::hash<float>()(v.y);
        return h1 ^ (h2 << 1);
    }
};

class Pathfinding 
{
public:
    Pathfinding();

    void initNodes(int width, int height);
    void draw(sf::RenderWindow& window);
    void handleInput();
    void findPath(Node* start, Node* end);
    void resetNodes();
    int heuristic(Node* start, Node* end);
    void resetWalkable();
    void move(float deltaTime);
    void addNode(const sf::Vector2f& position);
    void connect(Node* node1, Node* node2);
    void disconnect(Node* node1, Node* node2);
    Node* getNode(const sf::Vector2f& position);


    std::unordered_map<sf::Vector2f, Node, Vector2fHash> nodes;
    Node* start_node;
    Node* end_node;
    Node* current_node;
    sf::Vector2f tile_size;
    int width;
    int height;
};