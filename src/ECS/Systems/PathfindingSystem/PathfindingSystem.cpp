#include "../../../stdafx.h"
#include "PathfindingSystem.hpp"
#include <Pathfinding/Pathfinding.h>

void PathfindingSystem::update(entt::registry& registry, Pathfinding& pathfinding, float deltaTime)
{
    auto view = registry.view<Components::Pathfinding, Components::Position, Components::Velocity>();

    for (auto entity : view)
    {
        auto& path_component    = view.get<Components::Pathfinding>(entity);
        auto& positionComponent = view.get<Components::Position>(entity);
        auto& velocityComponent = view.get<Components::Velocity>(entity);

        // If the path is empty or the current node index is out of bounds, skip this entity
        if (path_component.path.empty() || path_component.current_node_index >= path_component.path.size())
            continue; // Skip this entity

        Node* nextNode = path_component.path.at(path_component.current_node_index);

        // Calculate the direction vector to the next node
        sf::Vector2f direction = nextNode->position - positionComponent.position;

        // Calculate the distance to the next node
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // If the distance is smaller than the speed, move directly to the node
        if (distance <= velocityComponent.speed * deltaTime)
        {
            positionComponent.position = nextNode->position;

            // If the current node is not the last node in the path, move to the next node
            if (path_component.current_node_index < path_component.path.size() - 1)
            {
                path_component.current_node_index++;
            }
            else
            {
                // If the entity has reached the end node, you can trigger a new pathfinding operation
                pathfinding.start_node = nextNode; // Set the current node as the new start node

                if (pathfinding.end_node)
                {
                    pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
                    path_component.path = pathfinding.path(); // Update the path component with the new path
                    path_component.current_node_index = 0;    // Reset the current node index
                }
            }
        }
        else
        {
            // Normalize the direction vector
            direction = direction / distance;

            // Calculate the velocity vector
            sf::Vector2f velocity = direction * velocityComponent.speed * deltaTime;

            // Update the position based on the velocity
            positionComponent.position += velocity;
        }
    }
}
