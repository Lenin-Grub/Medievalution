#pragma once
#include "../ISystem.hpp"

#include "../../../Pathfinding/Pathfinding.h"

class HandleInputSystem
{
public:
    static entt::entity getSquadLeader(entt::registry& registry, uint32_t squad_id);
    static void update(entt::registry& registry, Pathfinding& pathfinding);
};