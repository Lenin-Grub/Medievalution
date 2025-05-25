#include "PathfindingSystem.hpp"
#include <Pathfinding/Pathfinding.h>

void PathfindingSystem::update(entt::registry& registry, Pathfinding& global_pathfinding, float deltaTime)
{
    auto view = registry.view<Components::Pathfinding, Components::Position, Components::Velocity>();
    for (auto entity : view)
    {
        auto& path_component = registry.get<Components::Pathfinding>(entity);
        auto& positionComponent = registry.get<Components::Position>(entity);
        auto& velocityComponent = registry.get<Components::Velocity>(entity);

        if (path_component.path.empty() || path_component.current_node_index >= path_component.path.size())
        {
            if (registry.try_get<Components::State>(entity))
            {
                auto& state = registry.get<Components::State>(entity);
                if (state.state == Components::CharacterState::Move)
                {
                    state.state = Components::CharacterState::Idle;
                }
            }
            continue;
        }

        Node* nextNode = path_component.path[path_component.current_node_index];
        sf::Vector2f direction = nextNode->position - positionComponent.position;

        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance <= velocityComponent.speed * deltaTime)
        {
            positionComponent.position = nextNode->position;

            if (path_component.current_node_index < path_component.path.size() - 1)
            {
                path_component.current_node_index++;
            }
            else
            {
                path_component.start_node = nextNode;

                if (path_component.end_node)
                {
                    global_pathfinding.findPath(path_component.start_node, path_component.end_node);
                    path_component.path = global_pathfinding.path();
                    path_component.current_node_index = 0;
                }
            }
        }
        else
        {
            direction /= distance;
            sf::Vector2f velocity = direction * velocityComponent.speed * deltaTime;
            positionComponent.position += velocity;

            if (auto* state = registry.try_get<Components::State>(entity))
            {
                if (state->state != Components::CharacterState::Move)
                {
                    state->state = Components::CharacterState::Move;
                }
            }
        }
    }
}