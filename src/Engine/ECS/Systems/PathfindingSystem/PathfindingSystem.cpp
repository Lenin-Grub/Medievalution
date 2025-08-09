#include "PathfindingSystem.hpp"
#include <Pathfinding/Pathfinding.h>

void PathfindingSystem::update(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time)
{
    // --- Шаг 1: Сбросим все узлы как проходимые ---
    global_pathfinding.resetWalkable();

    // --- Шаг 2: Пометим узлы, занятые юнитами, как непроходимые ---
    auto all_units = registry.view<Components::Position>();
    for (auto entity : all_units)
    {
        auto& pos = registry.get<Components::Position>(entity);
        Node* node = global_pathfinding.getNodeByPosition(pos.position);
        if (node)
        {
            node->walkable = false;
        }
    }

    // --- Шаг 3: Обновляем путь и движение для каждого юнита с компонентом Pathfinding ---
    auto view = registry.view<Components::Pathfinding, Components::Position, Components::Velocity>();

    for (auto entity : view)
    {
        auto& path_comp = registry.get<Components::Pathfinding>(entity);
        auto& pos_comp = registry.get<Components::Position>(entity);
        auto& vel_comp = registry.get<Components::Velocity>(entity);

        if (path_comp.path.empty() || path_comp.current_node_index >= path_comp.path.size())
        {
            // Если путь закончился, и есть конечная точка — пересчитаем путь
            if (path_comp.end_node)
            {
                Node* start_node = global_pathfinding.getNodeByPosition(pos_comp.position);
                if (start_node && start_node != path_comp.end_node)
                {
                    // ВАЖНО: временно разрешим стоять на старте (иначе A* не запустится)
                    bool was_walkable = start_node->walkable;
                    start_node->walkable = true;

                    // Также временно делаем конечную точку проходимой
                    bool end_was_walkable = path_comp.end_node->walkable;
                    path_comp.end_node->walkable = true;

                    global_pathfinding.findPath(start_node, path_comp.end_node);
                    path_comp.path = global_pathfinding.path();
                    path_comp.current_node_index = 0;

                    // Восстанавливаем оригинальное состояние
                    start_node->walkable = was_walkable;
                    path_comp.end_node->walkable = end_was_walkable;

                    // Если путь не найден, ставим юнит в состояние покоя
                    if (path_comp.path.empty())
                    {
                        path_comp.end_node = nullptr;
                        setIdleStateIfMoving(registry, entity);
                        continue;
                    }
                }
            }
            else
            {
                setIdleStateIfMoving(registry, entity);
            }
            continue;
        }

        Node* next_node = path_comp.path[path_comp.current_node_index];
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
    auto& pos_comp = registry.get<Components::Position>(entity);

    Node* next_node = path_comp.path[path_comp.current_node_index];
    pos_comp.position = next_node->position;

    // Проверяем, если это последняя нода в пути
    if (path_comp.current_node_index >= path_comp.path.size() - 1)
    {
        // Достигли конца текущего пути
        LOG_INFO("Unit reached end of path at position ({0}, {1})", pos_comp.position.x, pos_comp.position.y);

        path_comp.start_node = next_node;

        // Проверяем, есть ли конечная точка и достигли ли мы её
        bool target_reached = false;
        if (path_comp.end_node)
        {
            // Проверяем, находимся ли мы в конечной точке (с небольшой погрешностью)
            const float epsilon = 5.0f; // допустимая погрешность
            float dx = std::abs(pos_comp.position.x - path_comp.end_node->position.x);
            float dy = std::abs(pos_comp.position.y - path_comp.end_node->position.y);

            if (dx <= epsilon && dy <= epsilon)
            {
                target_reached = true;
                LOG_INFO("Unit reached target destination!");
            }
        }

        if (target_reached || !path_comp.end_node)
        {
            // Цель достигнута или нет цели - ставим idle
            path_comp.path.clear();
            path_comp.current_node_index = 0;
            setIdleStateIfMoving(registry, entity);
            LOG_INFO("Setting unit to IDLE state");
        }
        else
        {
            // Есть цель, но еще не достигли - пересчитываем путь
            global_pathfinding.findPath(path_comp.start_node, path_comp.end_node);
            path_comp.path = global_pathfinding.path();
            path_comp.current_node_index = 0;
            if (!path_comp.path.empty())
            {
                LOG_INFO("Recalculating path to target");
            }
            else
            {
                LOG_INFO("No path found to target, setting IDLE");
                setIdleStateIfMoving(registry, entity);
            }
        }
    }
    else
    {
        // Переходим к следующей ноде
        path_comp.current_node_index++;
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

std::vector<Node*> PathfindingSystem::findNearbyEmptyNodes(Pathfinding& pathfinding, const sf::Vector2f& target_position, size_t count)
{
    std::vector<Node*> result;
    if (count == 0) return result;

    // Получаем центральную ноду
    Node* center_node = pathfinding.getNodeByPosition(target_position);
    if (!center_node) return result;

    // Собираем все доступные ноды в порядке увеличения расстояния
    std::vector<std::pair<float, Node*>> nodes_with_distance;

    // Проходим по всем нодам в сетке
    for (const auto& pair : pathfinding.getNodes())
    {
        Node* node = const_cast<Node*>(&pair.second);

        // Проверяем, что нода проходима
        if (node->walkable)
        {
            // Вычисляем расстояние до целевой позиции
            sf::Vector2f delta = node->position - target_position;
            float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

            nodes_with_distance.push_back({ distance, node });
        }
    }

    // Сортируем по расстоянию
    std::sort(nodes_with_distance.begin(), nodes_with_distance.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });

    // Берем нужное количество нод
    for (size_t i = 0; i < nodes_with_distance.size() && result.size() < count; ++i)
    {
        result.push_back(nodes_with_distance[i].second);
    }

    return result;
}

void PathfindingSystem::assignFormationPositions(entt::registry& registry, Pathfinding& global_pathfinding,
    const std::vector<entt::entity>& selected_units,
    const sf::Vector2f& target_position)
{
    if (selected_units.empty()) return;

    // Сначала собираем текущие позиции всех выделенных юнитов
    std::set<Node*> occupied_nodes;
    for (auto entity : selected_units)
    {
        auto& pos_comp = registry.get<Components::Position>(entity);
        Node* current_node = global_pathfinding.getNodeByPosition(pos_comp.position);
        if (current_node)
        {
            occupied_nodes.insert(current_node);
        }
    }

    // Находим свободные ноды рядом с целевой позицией
    auto target_nodes = findNearbyEmptyNodes(global_pathfinding, target_position, selected_units.size());

    // Назначаем ноды юнитам
    for (size_t i = 0; i < selected_units.size() && i < target_nodes.size(); ++i)
    {
        auto entity = selected_units[i];
        auto& path_comp = registry.get<Components::Pathfinding>(entity);
        auto& pos_comp = registry.get<Components::Position>(entity);

        Node* target_node = target_nodes[i];
        Node* start_node = global_pathfinding.getNodeByPosition(pos_comp.position);

        if (start_node && target_node && start_node != target_node)
        {
            // Назначаем пути
            path_comp.end_node = target_node;
            path_comp.start_node = start_node;

            // Временно делаем целевую ноду проходимой, но сохраняем оригинальное состояние
            bool target_was_walkable = target_node->walkable;
            target_node->walkable = true;

            // Также временно делаем проходимыми ноды, занятые другими юнитами из группы
            std::vector<std::pair<Node*, bool>> temporarily_made_walkable;
            for (Node* occupied_node : occupied_nodes)
            {
                if (occupied_node != start_node) // Не делаем проходимой стартовую позицию текущего юнита
                {
                    bool was_walkable = occupied_node->walkable;
                    occupied_node->walkable = true;
                    temporarily_made_walkable.push_back({ occupied_node, was_walkable });
                }
            }

            // Расчет пути
            global_pathfinding.findPath(path_comp.start_node, path_comp.end_node);
            path_comp.path = global_pathfinding.path();
            path_comp.current_node_index = 0;

            // Восстанавливаем оригинальное состояние нод
            target_node->walkable = target_was_walkable;
            for (const auto& [node, original_state] : temporarily_made_walkable)
            {
                node->walkable = original_state;
            }
        }
    }
}