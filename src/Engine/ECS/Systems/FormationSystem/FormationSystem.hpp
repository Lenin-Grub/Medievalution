#pragma once
#include "../ISystem.hpp"

#include "../../../Pathfinding/Pathfinding.h"

class FormationSystem
{
public:
    static void update(entt::registry& registry, Pathfinding& global_pathfinding);
};