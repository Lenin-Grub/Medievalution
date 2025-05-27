#pragma once
#include "../ISystem.hpp"

class Pathfinding;
class Node;

struct PathfindingSystem 
{
    static void update(entt::registry& registry, Pathfinding& pathfinding, float deltaTime);

private:
    static void setIdleStateIfMoving(entt::registry& registry, entt::entity entity);
    static void reachNextNode(entt::registry& registry, entt::entity entity, Pathfinding& global_pathfinding);
    static void moveTowardsNextNode(entt::registry& registry, entt::entity entity, const sf::Vector2f& direction, float distance, float delta_time);
    static void setCharacterState(entt::registry& registry, entt::entity entity);
};