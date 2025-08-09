#include "HandleInputSystem.hpp"
#include "../PathfindingSystem/PathfindingSystem.hpp" // Добавьте это для доступа к функциям формации

entt::entity HandleInputSystem::getSquadLeader(entt::registry& registry, uint32_t squad_id)
{
    auto squad_view = registry.view<Components::Squad>();

    for (auto entity : squad_view)
    {
        auto& squad = registry.get<Components::Squad>(entity);
        if (squad.squad_id == squad_id && squad.is_leader)
        {
            return entity;
        }
    }

    return entt::null;
}

void HandleInputSystem::update(entt::registry& registry, Pathfinding& global_pathfinding)
{
    // Обработка управления выделенными юнитами с клавиатуры
    auto control_view = registry.view<Components::Control, Components::Selectable, Components::Pathfinding, Components::Position>();

    for (auto entity : control_view)
    {
        auto& control = registry.get<Components::Control>(entity);
        auto& selectable = registry.get<Components::Selectable>(entity);

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

    // Обработка кликов мыши для перемещения
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        // Собираем всех выделенных юнитов
        std::vector<entt::entity> selected_units;
        auto selectable_view = registry.view<Components::Pathfinding, Components::Position, Components::Selectable>();

        for (auto entity : selectable_view)
        {
            auto& selectable = registry.get<Components::Selectable>(entity);
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
                // Проверяем, есть ли среди выделенных юнитов отряды
                bool has_squads = false;
                std::unordered_map<uint32_t, std::vector<entt::entity>> squads;

                for (auto entity : selected_units)
                {
                    if (registry.all_of<Components::Squad>(entity))
                    {
                        auto& squad = registry.get<Components::Squad>(entity);
                        if (squad.squad_id != 0)
                        {
                            has_squads = true;
                            squads[squad.squad_id].push_back(entity);
                        }
                    }
                }

                if (has_squads)
                {
                    // Обрабатываем отряды - устанавливаем цель только лидерам
                    for (const auto& [squad_id, squad_members] : squads)
                    {
                        entt::entity leader = getSquadLeader(registry, squad_id);
                        if (leader != entt::null && registry.valid(leader))
                        {
                            auto& leader_path = registry.get<Components::Pathfinding>(leader);
                            auto& leader_pos = registry.get<Components::Position>(leader);

                            leader_path.end_node = target_node;
                            leader_path.start_node = global_pathfinding.getNodeByPosition(leader_pos.position);

                            if (leader_path.start_node && leader_path.end_node)
                            {
                                // Временно делаем целевую ноду проходимой для расчета пути
                                bool target_was_walkable = target_node->walkable;
                                target_node->walkable = true;

                                // Также временно делаем стартовую ноду проходимой
                                bool start_was_walkable = leader_path.start_node->walkable;
                                leader_path.start_node->walkable = true;

                                global_pathfinding.findPath(leader_path.start_node, leader_path.end_node);
                                leader_path.path = global_pathfinding.path();
                                leader_path.current_node_index = 0;

                                // Восстанавливаем состояние нод
                                target_node->walkable = target_was_walkable;
                                leader_path.start_node->walkable = start_was_walkable;

                                // Проверяем, найден ли путь
                                if (leader_path.path.empty())
                                {
                                    // Путь не найден - сбрасываем
                                    leader_path.end_node = nullptr;
                                    leader_path.start_node = nullptr;
                                    leader_path.current_node_index = 0;

                                    // Устанавливаем состояние Idle
                                    if (auto* state = registry.try_get<Components::State>(leader))
                                    {
                                        if (state->state == Components::CharacterState::Move)
                                        {
                                            state->state = Components::CharacterState::Idle;
                                        }
                                    }
                                }
                                else
                                {
                                    // Устанавливаем состояние Move для лидера
                                    if (auto* state = registry.try_get<Components::State>(leader))
                                    {
                                        if (state->state != Components::CharacterState::Move)
                                        {
                                            state->state = Components::CharacterState::Move;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if (selected_units.size() == 1)
                {
                    // Один юнит без отряда - обычное поведение
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

                        // Также временно делаем стартовую ноду проходимой
                        bool start_was_walkable = path_component.start_node->walkable;
                        path_component.start_node->walkable = true;

                        global_pathfinding.findPath(path_component.start_node, path_component.end_node);
                        path_component.path = global_pathfinding.path();
                        path_component.current_node_index = 0;

                        // Восстанавливаем состояние нод
                        target_node->walkable = target_was_walkable;
                        path_component.start_node->walkable = start_was_walkable;

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
                    // Группа юнитов без отряда - используем формацию
                    // (если у вас есть PathfindingSystem::assignFormationPositions)
                    // Пока просто устанавливаем путь каждому юниту
                    for (auto entity : selected_units)
                    {
                        auto& path_component = registry.get<Components::Pathfinding>(entity);
                        auto& position_component = registry.get<Components::Position>(entity);

                        path_component.end_node = target_node;
                        path_component.start_node = global_pathfinding.getNodeByPosition(position_component.position);

                        if (path_component.start_node && path_component.end_node)
                        {
                            // Временно делаем целевую ноду проходимой
                            bool target_was_walkable = target_node->walkable;
                            target_node->walkable = true;

                            // Также временно делаем стартовую ноду проходимой
                            bool start_was_walkable = path_component.start_node->walkable;
                            path_component.start_node->walkable = true;

                            global_pathfinding.findPath(path_component.start_node, path_component.end_node);
                            path_component.path = global_pathfinding.path();
                            path_component.current_node_index = 0;

                            // Восстанавливаем состояние нод
                            target_node->walkable = target_was_walkable;
                            path_component.start_node->walkable = start_was_walkable;
                        }
                    }
                }
            }
        }
    }
}
