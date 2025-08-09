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

    /// @brief Finds a path from the start node to the end node using a pathfinding algorithm.
    /// @param start The start node.
    /// @param end The end node.
    void findPath(Node* start, Node* end);

    /// @brief Resets the state of all nodes.
    void resetNodes();

    /// @brief Resets the walkable state of all nodes.
    void resetWalkable();

    /// @brief Adds a new node at the specified position.
    /// @param position The position of the new node.
    void addNode(const sf::Vector2f& position);

    void removeNode(const sf::Vector2f& position);

    /// @brief Connects two nodes, allowing pathfinding between them.
    /// @param node1 The first node to connect.
    /// @param node2 The second node to connect.
    void connect(Node* node1, Node* node2, float cost);

    /// @brief Disconnects two nodes, preventing pathfinding between them.
    /// @param node1 The first node to disconnect.
    /// @param node2 The second node to disconnect.
    void disconnect(Node* node1, Node* node2);

    Node* getNodeByGridPosition(sf::Vector2i pos);

    /// @brief Gets the node at the specified position.
    /// @param position The position of the node to retrieve.
    /// @return The node at the specified position, or nullptr if not found.
    Node* getNode(const sf::Vector2f& position);

    Node* getNodeByPosition(const sf::Vector2f& worldPos);

    const std::unordered_map<sf::Vector2f, Node, Vector2fHash>& getNodes() const;

    std::vector<Node*> path() const;

    Node* getNodeByMousePosition(const sf::Vector2f& mousePosition);

    sf::Vector2i getMouseGridPosition();

    void setPathMode(PathMode mode);

    const PathMode& getPathMode() const;

private:

    /// @brief Computes the heuristic value between two nodes.
    /// @param start The start node.
    /// @param end The end node.
    /// @return The heuristic value.
    int heuristic(Node* start, Node* end, HeuristicType type = HeuristicType::Diagonal);

public:

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
};


class PathfindingRenderer 
{
public:
    static void render(const Pathfinding& pathfinding, sf::RenderWindow& window);

private:
    static void drawConnections(const Pathfinding& pathfinding, sf::RenderWindow& window);
    static void drawNodes(const Pathfinding& pathfinding, sf::RenderWindow& window);
    static void drawPath(const Pathfinding& pathfinding, sf::RenderWindow& window);
    static void drawNodeCost(const Pathfinding& pathfinding, sf::RenderWindow& window, const Node& node);
    static sf::Color getCostColor(float cost, float maxCost = 10.0f);

private:
    //sf::Font font;
};

class PathfindingInputSystem
{
public:
    static void handleInput(Pathfinding& pathfinding);
};

class GridSystem 
{
public:
    static void generateGrid(Pathfinding& pathfinding, int width, int height);
    static void connectNeighbors(Pathfinding& pathfinding, int x, int y, int mapWidth, int mapHeight, bool connectDiagonals = true);
    static sf::Vector2i getGridPosition(const Pathfinding& pathfinding, const sf::Vector2f& mousePosition);
};