#pragma once
#include "../ISystem.hpp"
#include <Pathfinding/Pathfinding.h>

class HandleInputSystem
{
public:
    static void update(entt::registry& registry, Pathfinding& pathfinding);
    static void assignFormationPositions(entt::registry& registry, Pathfinding& global_pathfinding, const std::vector<entt::entity>& selected_units, const sf::Vector2f& target_position);
    static void moveSquadUnit(entt::registry& registry, Pathfinding& global_pathfinding, entt::entity entity, Node* target_node);
};