#pragma once
#include "../ISystem.hpp"

#include "../../../Pathfinding/Pathfinding.h"

class HandleInputSystem
{
public:
    static void update(entt::registry& registry, Pathfinding& pathfinding);
};