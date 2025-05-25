#include "HandleInputSystem.hpp"

void HandleInputSystem::update(entt::registry& registry, Pathfinding& global_pathfinding) 
{
    auto view = registry.view<Components::Control, Components::Selectable, Components::Pathfinding, Components::Position>();

    for (auto entity : view) 
    {
        auto& control            = view.get<Components::Control>    (entity);
        auto& selectable         = view.get<Components::Selectable> (entity);
        auto& path_component     = view.get<Components::Pathfinding>(entity);
        auto& position_component = view.get<Components::Position>   (entity);

        if (selectable.is_selected == false) continue;

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
        auto view = registry.view<Components::Pathfinding, Components::Position, Components::Selectable>();

        for (auto entity : view) 
        {
            auto& path_component     = view.get<Components::Pathfinding>(entity);
            auto& position_component = view.get<Components::Position>   (entity);
            auto& selectable         = view.get<Components::Selectable> (entity);

            if (selectable.is_selected == false) continue;

            // New path
            path_component.end_node   = global_pathfinding.getNodeByMousePosition(common::mouse_pos_view);
            path_component.start_node = global_pathfinding.getNode(position_component.position);

            if (path_component.start_node && path_component.end_node) 
            {
                global_pathfinding.findPath(path_component.start_node, path_component.end_node);
                path_component.path = global_pathfinding.path();
                path_component.current_node_index = 0;
            }
        }
    }
}