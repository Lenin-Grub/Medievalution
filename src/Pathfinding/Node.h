#pragma once
#include "../Common/Common.h"

struct Node
{
    bool               walkable   = false;
    bool               is_visited = false;
    float              gCost      = 0;
    float              hCost      = 0;
    float              fCost      = 0;
    Node*              parent     = nullptr;
    sf::Vector2f       position;
    std::vector<Node*> neighbors;

    bool operator()(Node* a, Node* b)
    {
        return a->fCost > b->fCost;
    }
};