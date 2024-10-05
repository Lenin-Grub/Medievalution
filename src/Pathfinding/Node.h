#pragma once
#include "../Common/Common.h"

struct Node
{
    bool               walkable   = false;      ///< @brief Indicates whether the node is walkable.
    bool               is_visited = false;      ///< @brief Indicates whether the node has been visited.
    float              gCost      = 0;          ///< @brief The cost of reaching this node from the starting node (g-cost).
    float              hCost      = 0;          ///< @brief The estimated cost of reaching the goal node from this node (h-cost).
    float              fCost      = 0;          ///< @brief The total cost of reaching this node (f-cost), calculated as g-cost + h-cost.
    Node*              parent     = nullptr;    ///< @brief The parent node of this node.
    sf::Vector2f       position;                ///< @brief The position of this node in 2D space.
    std::vector<Node*> neighbors;               ///< @brief A list of neighboring nodes.

    bool operator()(Node* a, Node* b)
    {
        return a->fCost > b->fCost;
    }
};