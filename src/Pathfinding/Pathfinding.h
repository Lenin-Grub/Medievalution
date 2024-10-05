#pragma once
#include "../Common/Common.h"
#include "Node.h"

class Pathfinding
{
public:
    Pathfinding();
    virtual ~Pathfinding() = default;

    /// @brief Initializes the grid with the specified width and height.
    /// @param width The width of the grid.
    /// @param height The height of the grid.
    void initNodes(int width, int height);

    /// @brief Draws the grid on the specified window.
    /// @param window The window on which to draw the grid.
    void draw(sf::RenderWindow& window);

    /// @brief Handles user input for the grid.
    void handleInput();

    /// @brief Finds a path between the specified start and end nodes.
    /// @param start The starting node.
    /// @param end The ending node.
    void findPath(Node* start, Node* end);

    /// @brief Resets the nodes in the grid to their initial state.
    void resetNodes();

    /// @brief Calculates the heuristic cost between two nodes.
    /// @param start The starting node.
    /// @param end The ending node.
    /// @return The estimated cost of reaching the end node from the start node.
    int  heuristic(Node* start, Node* end);

    Node* start_node;
    Node* end_node;

private:
    std::vector<std::vector<Node>> nodes;
    sf::Vector2f tile_size;
    int width;
    int height;
};