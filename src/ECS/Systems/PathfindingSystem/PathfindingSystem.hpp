#pragma once
#include "../ISystem.hpp"

class Pathfinding;
class Node;

class PathfindingSystem
{
public:

    static void update(entt::registry& registry, Pathfinding& pathfinding);
};