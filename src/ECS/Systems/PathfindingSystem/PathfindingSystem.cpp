#include "../../../stdafx.h"
#include "PathfindingSystem.hpp"
#include <Pathfinding/Pathfinding.h>

void PathfindingSystem::update(entt::registry& registry, Pathfinding& pathfinding) 
{
    auto view = registry.view<Component_Path, Component_Position>();

    for (auto entity : view)
    {
        auto& path_component    = view.get<Component_Path>(entity);
        auto& positionComponent = view.get<Component_Position>(entity);

        // If the path is empty or the current node index is out of bounds, skip this entity
        if (path_component.path.empty() || path_component.current_node_index >= path_component.path.size())
            continue; // Skip this entity

        Node* currentNode = path_component.path.at(path_component.current_node_index);

        // Move the entity to the current node's position
        positionComponent.position = currentNode->position;

        // Check if the entity has reached the current node
        if (positionComponent.position == currentNode->position)
        {
            // If the current node is not the last node in the path, move to the next node
            if (path_component.current_node_index < path_component.path.size() - 1)
                path_component.current_node_index++;
            else
            {
                // If the entity has reached the end node, you can trigger a new pathfinding operation
                // For example, you can set a new target position based on mouse input or some other logic
                pathfinding.start_node = currentNode; // Set the current node as the new start node

                if (pathfinding.end_node)
                {
                    pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
                    path_component.path = pathfinding.path(); // Update the path component with the new path
                    path_component.current_node_index = 0;    // Reset the current node index
                }
            }
        }
    }
}