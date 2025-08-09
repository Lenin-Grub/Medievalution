#include "HandleInputSystem.hpp"
#include "../PathfindingSystem/PathfindingSystem.hpp" // Добавьте это для доступа к функциям формации

void HandleInputSystem::update(entt::registry& registry, Pathfinding& global_pathfinding)
{
    auto view = registry.view<Components::Control, Components::Selectable, Components::Pathfinding, Components::Position>();

    for (auto entity : view)
    {
        auto& control = view.get<Components::Control>(entity);
        auto& selectable = view.get<Components::Selectable>(entity);
        auto& path_component = view.get<Components::Pathfinding>(entity);
        auto& position_component = view.get<Components::Position>(entity);

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
        // Собираем всех выделенных юнитов
        std::vector<entt::entity> selected_units;
        auto view = registry.view<Components::Pathfinding, Components::Position, Components::Selectable>();

        for (auto entity : view)
        {
            auto& selectable = view.get<Components::Selectable>(entity);
            if (selectable.is_selected)
            {
                selected_units.push_back(entity);
            }
        }

        if (!selected_units.empty())
        {
            // Получаем целевую позицию
            Node* target_node = global_pathfinding.getNodeByMousePosition(common::mouse_pos_view);
            if (target_node)
            {
                if (selected_units.size() == 1)
                {
                    // Один юнит - обычное поведение с проверкой достижимости
                    auto entity = selected_units[0];
                    auto& path_component = registry.get<Components::Pathfinding>(entity);
                    auto& position_component = registry.get<Components::Position>(entity);

                    path_component.end_node = target_node;
                    path_component.start_node = global_pathfinding.getNodeByPosition(position_component.position);

                    if (path_component.start_node && path_component.end_node)
                    {
                        // Временно делаем целевую ноду проходимой для расчета пути
                        bool target_was_walkable = target_node->walkable;
                        target_node->walkable = true;

                        global_pathfinding.findPath(path_component.start_node, path_component.end_node);
                        path_component.path = global_pathfinding.path();
                        path_component.current_node_index = 0;

                        // Восстанавливаем состояние целевой ноды
                        target_node->walkable = target_was_walkable;

                        // Проверяем, найден ли путь
                        if (path_component.path.empty())
                        {
                            // Путь не найден - ставим юнит в состояние покоя
                            path_component.end_node = nullptr;
                            path_component.start_node = nullptr;
                            path_component.current_node_index = 0;
                            // Устанавливаем состояние Idle
                            if (auto* state = registry.try_get<Components::State>(entity))
                            {
                                if (state->state == Components::CharacterState::Move)
                                {
                                    state->state = Components::CharacterState::Idle;
                                }
                            }
                        }
                    }
                }
                else
                {
                    // Группа юнитов - используем формацию
                    PathfindingSystem pathfinding_system;
                    pathfinding_system.assignFormationPositions(registry, global_pathfinding, selected_units, target_node->position);
                }
            }
        }
    }
}


