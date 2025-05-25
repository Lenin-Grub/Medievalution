#pragma once
#include <unordered_set>
#include <queue>

#include <Resource/ResourceManager.hpp>

#include "Common/Common.h"
#include "Node.h"

enum class PathMode 
{
    Regular,
    Isometric,
    NonDiscrete
};

enum class HeuristicType 
{ 
    Manhattan,
    Euclidean,
    Diagonal 
};

/// @brief Custom hash function for sf::Vector2f to use in unordered_map.
struct Vector2fHash
{
    /// @brief Computes the hash value for an sf::Vector2f object.
    /// @param v The sf::Vector2f object to hash.
    /// @return The computed hash value.
    std::size_t operator()(const sf::Vector2f& v) const 
    {
        std::size_t h1 = std::hash<float>()(v.x);
        std::size_t h2 = std::hash<float>()(v.y);
        return h1 ^ (h2 << 1);
    }
};

/// @brief Class for managing pathfinding operations on a grid of nodes.
class Pathfinding
{
public:
    /// @brief Default constructor.
    Pathfinding();

    /// @brief Initializes the nodes grid with the specified width and height.
    /// @param width The width of the grid.
    /// @param height The height of the grid.
    void initNodes(int width, int height);

    /// @brief Draws the nodes and paths on the given render window.
    /// @param window The render window to draw on.
    void draw(sf::RenderWindow& window);

    /// @brief Handles user input for pathfinding operations.
    void handleInput();

    /// @brief Finds a path from the start node to the end node using a pathfinding algorithm.
    /// @param start The start node.
    /// @param end The end node.
    void findPath(Node* start, Node* end);

    /// @brief Resets the state of all nodes.
    void resetNodes();

    /// @brief Computes the heuristic value between two nodes.
    /// @param start The start node.
    /// @param end The end node.
    /// @return The heuristic value.
    int heuristic(Node* start, Node* end, HeuristicType type = HeuristicType::Diagonal);

    /// @brief Resets the walkable state of all nodes.
    void resetWalkable();

    /// @brief Moves the current node based on the delta time.
    /// @param deltaTime The time elapsed since the last frame.
    void move(float deltaTime);

    /// @brief Adds a new node at the specified position.
    /// @param position The position of the new node.
    void addNode(const sf::Vector2f& position);

    /// @brief Connects two nodes, allowing pathfinding between them.
    /// @param node1 The first node to connect.
    /// @param node2 The second node to connect.
    void connect(Node* node1, Node* node2, float cost);

    /// @brief Connects two nodes, allowing pathfinding between them.
    /// @param node1 The first node to connect.
    /// @param node2 The second node to connect.
    void connectNeighbors(int x, int y, int mapWidth, int mapHeight, bool connectDiagonals = true);

    Node* getNodeByGridPosition(sf::Vector2i pos);

    void drawNodeCost(sf::RenderWindow& window, sf::Font& font);

    /// @brief Disconnects two nodes, preventing pathfinding between them.
    /// @param node1 The first node to disconnect.
    /// @param node2 The second node to disconnect.
    void disconnect(Node* node1, Node* node2);

    /// @brief Gets the node at the specified position.
    /// @param position The position of the node to retrieve.
    /// @return The node at the specified position, or nullptr if not found.
    Node* getNode(const sf::Vector2f& position);

    std::vector<Node*> path() const;

    Node* getNodeByMousePosition(const sf::Vector2f& mousePosition);

    Node* getRandomEndNode() const;

    sf::Vector2i getMouseGridPosition();

    void setPathMode(PathMode mode);

    /// @brief Map of nodes indexed by their position.
    std::unordered_map<sf::Vector2f, Node, Vector2fHash> nodes; 

    Node*        start_node;    ///< The start node for pathfinding.
    Node*        end_node;      ///< The end node for pathfinding.
    Node*        current_node;  ///< The current node in the pathfinding process.
    sf::Vector2f tile_size;     ///< The size of each tile in the grid.
    int          width;         ///< The width of the grid.
    int          height;        ///< The height of the grid.

    bool        is_path_visible;
    bool        is_nodes_visible;
    bool        is_connections_visible;
    bool        is_beginend_visible;

private:
    PathMode path_mode;
    sf::Font font;
};
