#include "../../../stdafx.h"
#include "PathfindingSystem.hpp"
#include <Pathfinding/Pathfinding.h>

void PathfindingSystem::update(entt::registry& registry, Pathfinding& global_pathfinding, float deltaTime)
{
    auto view = registry.view<Components::Pathfinding, Components::Position, Components::Velocity>();
    for (auto entity : view) 
    {
        auto& path_component    = view.get<Components::Pathfinding>(entity);
        auto& positionComponent = view.get<Components::Position>   (entity);
        auto& velocityComponent = view.get<Components::Velocity>   (entity);

        if (path_component.path.empty() || path_component.current_node_index >= path_component.path.size())
            continue;

        Node* nextNode = path_component.path.at(path_component.current_node_index);

        // Going to next node
        sf::Vector2f direction = nextNode->position - positionComponent.position;

        float nex_node_distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (nex_node_distance <= velocityComponent.speed * deltaTime) 
        {
            positionComponent.position = nextNode->position;

            if (path_component.current_node_index < path_component.path.size() - 1) 
                path_component.current_node_index++;
            else 
            {   // If finishing, can init new way
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
            direction /= nex_node_distance;
            sf::Vector2f velocity = direction * velocityComponent.speed * deltaTime;
            positionComponent.position += velocity;
        }
    }
}
