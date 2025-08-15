#pragma once
#include "../ISystem.hpp"

class Pathfinding;
class Node;

struct PathfindingSystem 
{
    static void update(entt::registry& registry, Pathfinding& pathfinding, float deltaTime);
    static void assignFormationPositions(entt::registry& registry, Pathfinding& global_pathfinding, const std::vector<entt::entity>& selected_units, const sf::Vector2f& target_position);
    static std::vector<Node*> findNearbyEmptyNodes(Pathfinding& pathfinding, const sf::Vector2f& target_position, size_t count);

private:
    static void setIdleStateIfMoving(entt::registry& registry, entt::entity entity);
    static void reachNextNode(entt::registry& registry, entt::entity entity, Pathfinding& global_pathfinding);
    static void moveTowardsNextNode(entt::registry& registry, entt::entity entity, const sf::Vector2f& direction, float distance, float delta_time);
    static void setCharacterState(entt::registry& registry, entt::entity entity);
};