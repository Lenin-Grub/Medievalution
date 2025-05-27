#include "PathfindingSystem.hpp"
#include <Pathfinding/Pathfinding.h>

void PathfindingSystem::update(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time)
{
    auto view = registry.view<Components::Pathfinding, Components::Position, Components::Velocity>();

    for (auto entity : view)
    {
        auto& path_comp = registry.get<Components::Pathfinding>(entity);

        if (path_comp.path.empty() || path_comp.current_node_index >= path_comp.path.size())
        {
            setIdleStateIfMoving(registry, entity);
            continue;
        }

        Node* next_node = path_comp.path[path_comp.current_node_index];
        auto& pos_comp  = registry.get<Components::Position>(entity);
        auto& vel_comp  = registry.get<Components::Velocity>(entity);

        sf::Vector2f direction = next_node->position - pos_comp.position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance <= vel_comp.speed * delta_time)
        {
            reachNextNode(registry, entity, global_pathfinding);
        }
        else
        {
            moveTowardsNextNode(registry, entity, direction, distance, delta_time);
            setCharacterState(registry, entity);
        }
    }
}

void PathfindingSystem::setIdleStateIfMoving(entt::registry& registry, entt::entity entity)
{
    if (auto* state = registry.try_get<Components::State>(entity))
    {
        if (state->state == Components::CharacterState::Move)
        {
            state->state = Components::CharacterState::Idle;
        }
    }
}

void PathfindingSystem::reachNextNode(entt::registry& registry, entt::entity entity, Pathfinding& global_pathfinding)
{
    auto& path_comp = registry.get<Components::Pathfinding>(entity);
    auto& pos_comp  = registry.get<Components::Position>(entity);

    Node* next_node = path_comp.path[path_comp.current_node_index];
    pos_comp.position = next_node->position;

    if (path_comp.current_node_index < path_comp.path.size() - 1)
    {
        path_comp.current_node_index++;
    }
    else
    {
        path_comp.start_node = next_node;
        if (path_comp.end_node)
        {
            global_pathfinding.findPath(path_comp.start_node, path_comp.end_node);
            path_comp.path = global_pathfinding.path();
            path_comp.current_node_index = 0;
        }
    }
}

void PathfindingSystem::moveTowardsNextNode(entt::registry& registry, entt::entity entity, const sf::Vector2f& direction, float distance, float delta_time)
{
    auto& pos_comp = registry.get<Components::Position>(entity);
    auto& vel_comp = registry.get<Components::Velocity>(entity);

    if (distance > 0.0f)
    {
        sf::Vector2f normalizedDirection = direction / distance;
        pos_comp.position += normalizedDirection * vel_comp.speed * delta_time;
    }
}

void PathfindingSystem::setCharacterState(entt::registry& registry, entt::entity entity)
{
    if (auto* state = registry.try_get<Components::State>(entity))
    {
        if (state->state != Components::CharacterState::Move)
        {
            state->state = Components::CharacterState::Move;
        }
    }
}