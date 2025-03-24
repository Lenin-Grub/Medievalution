#include "../../../stdafx.h"
#include "HandleInputSystem.hpp"

void HandleInputSystem::update(entt::registry& registry, Pathfinding& pathfinding)
{
    auto view = registry.view<Components::Control>();
    for (auto entity : view)
    {
        auto& control = view.get<Components::Control>(entity);

        control.direction = sf::Vector2f(0.0f, 0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            control.direction.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            control.direction.y += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            control.direction.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            control.direction.x += 1.0f;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        auto view2 = registry.view<Components::Pathfinding, Components::Position>();
        for (auto entity : view2)
        {
            auto& path_component     = view2.get<Components::Pathfinding>(entity);
            auto& position_component = view2.get<Components::Position>(entity);

          // if (pathfinding.current_node != nullptr && pathfinding.start_node != nullptr && pathfinding.end_node != nullptr)
            {
                // Set the new end node based on the mouse position
                pathfinding.end_node = pathfinding.getNodeByMousePosition(common::mouse_pos_view);

                // Update the start node to the current position of the entity
                pathfinding.start_node = pathfinding.getNode(position_component.position);
            }

            // If start and end nodes are valid, find a new path
            if (pathfinding.start_node && pathfinding.end_node)
            {
                pathfinding.findPath(pathfinding.start_node, pathfinding.end_node);
                path_component.path = pathfinding.path();
                path_component.current_node_index = 0; // Reset the current node index
            }
        }
    }
}