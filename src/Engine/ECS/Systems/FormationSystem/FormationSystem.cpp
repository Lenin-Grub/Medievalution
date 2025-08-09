#include  "FormationSystem.hpp"
#include  "FormationSystem.hpp"

sf::Vector2f SquadMovementSystem::calculateFormationOffset(uint32_t formation_index, const sf::Vector2f& move_direction, float spacing, Components::FormationType formation_type)
{
    // Нормализуем вектор направления движения
    sf::Vector2f normalized_dir = move_direction;
    float length = std::sqrt(move_direction.x * move_direction.x + move_direction.y * move_direction.y);
    if (length > 0.001f)
    {
        normalized_dir.x /= length;
        normalized_dir.y /= length;
    }
    else
    {
        normalized_dir = sf::Vector2f(1.0f, 0.0f);
    }

    // Вычисляем перпендикулярное направление
    sf::Vector2f perp_dir(-normalized_dir.y, normalized_dir.x);

    switch (formation_type)
    {
    case Components::FormationType::Line:
        return calculateLineOffset(formation_index, normalized_dir, perp_dir, spacing);

    case Components::FormationType::Square:
        return calculateSquareOffset(formation_index, normalized_dir, perp_dir, spacing);

    case Components::FormationType::Circle:
        return calculateCircleOffset(formation_index, normalized_dir, perp_dir, spacing);

    case Components::FormationType::Diamond:
        return calculateDiamondOffset(formation_index, normalized_dir, perp_dir, spacing);

    case Components::FormationType::Triangle:
        return calculateTriangleOffset(formation_index, normalized_dir, perp_dir, spacing);

    default:
        return calculateLineOffset(formation_index, normalized_dir, perp_dir, spacing);
    }
}

sf::Vector2f SquadMovementSystem::calculateLineOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing)
{
    if (formation_index == 0)
    {
        // Лидер - впереди
        return sf::Vector2f(forward.x * spacing * 2, forward.y * spacing * 2);
    }
    else
    {
        // Остальные - в линии позади
        int line_pos = static_cast<int>(formation_index) - 5;
        return sf::Vector2f(
            -forward.x * spacing + right.x * line_pos * spacing,
            -forward.y * spacing + right.y * line_pos * spacing
        );
    }
}

sf::Vector2f SquadMovementSystem::calculateSquareOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing)
{
    if (formation_index == 0)
    {
        // Лидер - перед квадратом
        return sf::Vector2f(forward.x * spacing * 2, forward.y * spacing * 2);
    }
    else
    {
        // Остальные юниты формируют квадрат позади точки, где должен быть лидер
        int index_in_square = formation_index - 1;

        // Создаем квадрат 3x3 позади "виртуальной" позиции лидера
        int row = index_in_square / 3;
        int col = index_in_square % 3;

        // Центрируем квадрат
        int center_row = 1; // Центр квадрата 3x3
        int center_col = 1;

        // Позиции квадрата позади виртуальной позиции лидера
        float square_offset_x = right.x * (col - center_col) * spacing + forward.x * (row - center_row) * spacing;
        float square_offset_y = right.y * (col - center_col) * spacing + forward.y * (row - center_row) * spacing;

        // Смещаем весь квадрат позади лидера
        float behind_leader_x = -forward.x * spacing * 2;
        float behind_leader_y = -forward.y * spacing * 2;

        return sf::Vector2f(square_offset_x + behind_leader_x, square_offset_y + behind_leader_y);
    }
}

sf::Vector2f SquadMovementSystem::calculateCircleOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing)
{
    if (formation_index == 0)
    {
        // Лидер - в центре круга
        return sf::Vector2f(0, 0);
    }
    else
    {
        // Распределяем остальных по окружности вокруг лидера
        int units_in_circle = 9;
        float angle_step = 2.0f * 3.14159f / units_in_circle;
        float angle = (formation_index - 1) * angle_step;
        float radius = spacing * 2.0f;

        // Окружность вокруг лидера
        sf::Vector2f circle_offset(
            right.x * radius * cos(angle) - forward.x * radius * sin(angle),
            right.y * radius * cos(angle) - forward.y * radius * sin(angle)
        );

        return circle_offset;
    }
}

sf::Vector2f SquadMovementSystem::calculateDiamondOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing)
{
    if (formation_index == 0)
    {
        // Лидер - в центре ромба
        return sf::Vector2f(0, 0);
    }
    else
    {
        // Ромб: создаем ромбовидную структуру вокруг лидера
        int index_in_diamond = formation_index - 1;

        // Ромб с 3 рядами: центр, 4 угла, 4 внешних
        if (index_in_diamond < 4) {
            // Первый "круг" - 4 позиции по диагоналям
            float angle = index_in_diamond * 3.14159f / 2.0f; // 0, 90, 180, 270 градусов
            float radius = spacing;
            return sf::Vector2f(
                right.x * radius * cos(angle) - forward.x * radius * sin(angle),
                right.y * radius * cos(angle) - forward.y * radius * sin(angle)
            );
        }
        else {
            // Второй "круг" - 4 внешние позиции
            float angle = (index_in_diamond - 4) * 3.14159f / 2.0f;
            float radius = spacing * 2.0f;
            return sf::Vector2f(
                right.x * radius * cos(angle) - forward.x * radius * sin(angle),
                right.y * radius * cos(angle) - forward.y * radius * sin(angle)
            );
        }
    }
}

sf::Vector2f SquadMovementSystem::calculateTriangleOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing)
{
    if (formation_index == 0)
    {
        // Лидер - в центре основания (сзади)
        return sf::Vector2f(-forward.x * spacing, -forward.y * spacing);
    }
    else
    {
        // Клин: юниты расходятся вперед от лидера
        int index_in_triangle = formation_index - 1;

        // Вычисляем ряд и позицию в ряду
        int row = 0;
        int total_in_previous_rows = 0;
        while (total_in_previous_rows + (row * 2 + 1) <= index_in_triangle) {
            total_in_previous_rows += (row * 2 + 1);
            row++;
        }
        int pos_in_row = index_in_triangle - total_in_previous_rows;

        // Центрируем ряд
        int row_width = row * 2 + 1;
        int center_offset = row;

        // Располагаем клином впереди лидера
        return sf::Vector2f(
            forward.x * (row + 1) * spacing + right.x * (pos_in_row - center_offset) * spacing,
            forward.y * (row + 1) * spacing + right.y * (pos_in_row - center_offset) * spacing
        );
    }
}

void SquadMovementSystem::update(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time)
{
    // Сначала сбрасываем все ноды как проходимые
    global_pathfinding.resetWalkable();

    // Затем помечаем занятые позиции как непроходимые
    auto position_view = registry.view<Components::Position>();
    for (auto entity : position_view)
    {
        auto& pos = registry.get<Components::Position>(entity);
        Node* node = global_pathfinding.getNodeByPosition(pos.position);
        if (node)
        {
            node->walkable = false;
        }
    }

    // Обновляем формации отрядов
    updateSquadFormation(registry, global_pathfinding);

    // Избегаем столкновений между отрядами
    avoidSquadCollisions(registry, global_pathfinding);

    // Обновляем движение всех юнитов
    auto view = registry.view<Components::Pathfinding, Components::Position, Components::Velocity>();

    for (auto entity : view)
    {
        auto& path_comp = registry.get<Components::Pathfinding>(entity);
        auto& pos_comp = registry.get<Components::Position>(entity);
        auto& vel_comp = registry.get<Components::Velocity>(entity);

        if (path_comp.path.empty() || path_comp.current_node_index >= path_comp.path.size())
        {
            if (auto* state = registry.try_get<Components::State>(entity))
            {
                if (state->state == Components::CharacterState::Move)
                {
                    state->state = Components::CharacterState::Idle;
                }
            }
            continue;
        }

        Node* next_node = path_comp.path[path_comp.current_node_index];
        sf::Vector2f direction = next_node->position - pos_comp.position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance <= vel_comp.speed * delta_time)
        {
            pos_comp.position = next_node->position;

            if (path_comp.current_node_index < path_comp.path.size() - 1)
            {
                path_comp.current_node_index++;
            }
            else
            {
                if (auto* state = registry.try_get<Components::State>(entity))
                {
                    if (state->state == Components::CharacterState::Move)
                    {
                        state->state = Components::CharacterState::Idle;
                    }
                }
            }
        }
        else
        {
            if (distance > 0.0f)
            {
                sf::Vector2f normalizedDirection = direction / distance;
                pos_comp.position += normalizedDirection * vel_comp.speed * delta_time;
            }

            if (auto* state = registry.try_get<Components::State>(entity))
            {
                if (state->state == Components::CharacterState::Idle && !path_comp.path.empty())
                {
                    state->state = Components::CharacterState::Move;
                }
            }
        }
    }
}

void SquadMovementSystem::updateSquadFormation(entt::registry& registry, Pathfinding& global_pathfinding)
{
    std::unordered_set<uint32_t> processed_squads;
    auto squad_view = registry.view<Components::Squad>();

    for (auto entity : squad_view)
    {
        auto& squad = registry.get<Components::Squad>(entity);

        if (squad.squad_id != 0 && processed_squads.find(squad.squad_id) == processed_squads.end())
        {
            entt::entity leader = getSquadLeader(registry, squad.squad_id);

            if (leader != entt::null && registry.valid(leader))
            {
                auto& leader_path = registry.get<Components::Pathfinding>(leader);
                auto& leader_pos = registry.get<Components::Position>(leader);

                if (leader_path.end_node && !leader_path.path.empty())
                {
                    sf::Vector2f leader_target = leader_path.end_node->position;
                    sf::Vector2f move_direction = leader_target - leader_pos.position;

                    auto members = getSquadMembers(registry, squad.squad_id);

                    for (auto member : members)
                    {
                        if (member == leader) continue;

                        auto& member_squad = registry.get<Components::Squad>(member);
                        auto& member_path = registry.get<Components::Pathfinding>(member);
                        auto& member_pos = registry.get<Components::Position>(member);

                        float spacing = 32.0f;

                        // ИСПОЛЬЗУЕМ ТИП ФОРМАЦИИ ИЗ КОМПОНЕНТА
                        sf::Vector2f formation_offset = calculateFormationOffset(
                            member_squad.formation_index, move_direction, spacing, member_squad.formation_type);

                        sf::Vector2f formation_target = leader_target + formation_offset;

                        Node* target_node = global_pathfinding.getNodeByPosition(formation_target);
                        Node* start_node = global_pathfinding.getNodeByPosition(member_pos.position);

                        if (target_node && start_node && start_node != target_node)
                        {
                            bool target_changed = (member_path.end_node != target_node);

                            if (target_changed || member_path.path.empty())
                            {
                                bool target_was_walkable = target_node->walkable;
                                bool start_was_walkable = start_node->walkable;

                                target_node->walkable = true;
                                start_node->walkable = true;

                                global_pathfinding.findPath(start_node, target_node);

                                auto new_path = global_pathfinding.path();
                                if (!new_path.empty())
                                {
                                    member_path.path = new_path;
                                    member_path.current_node_index = 0;
                                    member_path.end_node = target_node;
                                    member_path.start_node = start_node;

                                    if (auto* state = registry.try_get<Components::State>(member))
                                    {
                                        state->state = Components::CharacterState::Move;
                                    }
                                }

                                target_node->walkable = target_was_walkable;
                                start_node->walkable = start_was_walkable;
                            }
                        }
                    }
                }
            }

            processed_squads.insert(squad.squad_id);
        }
    }
}

void SquadMovementSystem::avoidSquadCollisions(entt::registry& registry, Pathfinding& global_pathfinding)
{
    // Получаем все отряды
    std::vector<uint32_t> squad_ids;
    std::unordered_set<uint32_t> processed_squads;

    auto squad_view = registry.view<Components::Squad>();
    for (auto entity : squad_view)
    {
        auto& squad = registry.get<Components::Squad>(entity);
        if (squad.squad_id != 0 && processed_squads.find(squad.squad_id) == processed_squads.end())
        {
            squad_ids.push_back(squad.squad_id);
            processed_squads.insert(squad.squad_id);
        }
    }

    // Проверяем столкновения и разрешаем их
    for (size_t i = 0; i < squad_ids.size(); ++i)
    {
        for (size_t j = i + 1; j < squad_ids.size(); ++j)
        {
            if (checkSquadCollision(registry, squad_ids[i], squad_ids[j]))
            {
                // Получаем лидеров
                entt::entity leader1 = getSquadLeader(registry, squad_ids[i]);
                entt::entity leader2 = getSquadLeader(registry, squad_ids[j]);

                if (leader1 != entt::null && leader2 != entt::null &&
                    registry.valid(leader1) && registry.valid(leader2))
                {
                    auto& path1 = registry.get<Components::Pathfinding>(leader1);
                    auto& path2 = registry.get<Components::Pathfinding>(leader2);

                    // Проверяем, движутся ли оба отряда к одной и той же цели
                    if (path1.end_node && path2.end_node && path1.end_node == path2.end_node)
                    {
                        // Получаем центры отрядов
                        sf::Vector2f center1 = getSquadCenter(registry, squad_ids[i]);
                        sf::Vector2f center2 = getSquadCenter(registry, squad_ids[j]);

                        // Вектор от первого ко второму отряду
                        sf::Vector2f direction = center2 - center1;
                        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                        if (distance > 0.001f)
                        {
                            // Нормализуем направление
                            direction.x /= distance;
                            direction.y /= distance;

                            // Небольшое смещение (меньше, чем раньше)
                            float offset_distance = 20.0f;
                            sf::Vector2f offset(direction.x * offset_distance, direction.y * offset_distance);

                            // Смещаем цель второго отряда немного в сторону от первого
                            sf::Vector2f new_target_pos = path2.end_node->position + offset;

                            // Находим ближайшую доступную ноду
                            Node* new_target_node = global_pathfinding.getNodeByPosition(new_target_pos);
                            if (new_target_node && new_target_node != path2.end_node)
                            {
                                // Проверяем, что новая позиция не слишком далеко от оригинальной цели
                                sf::Vector2f to_original = new_target_pos - path1.end_node->position;
                                float dist_to_original = std::sqrt(to_original.x * to_original.x + to_original.y * to_original.y);

                                if (dist_to_original < 100.0f) // Не дальше чем 100 пикселей от оригинальной цели
                                {
                                    path2.end_node = new_target_node;

                                    // Пересчитываем путь для второго лидера
                                    auto& leader2_pos = registry.get<Components::Position>(leader2);
                                    Node* start_node = global_pathfinding.getNodeByPosition(leader2_pos.position);
                                    if (start_node && start_node != new_target_node)
                                    {
                                        bool target_was_walkable = new_target_node->walkable;
                                        bool start_was_walkable = start_node->walkable;

                                        new_target_node->walkable = true;
                                        start_node->walkable = true;

                                        global_pathfinding.findPath(start_node, new_target_node);

                                        auto new_path = global_pathfinding.path();
                                        if (!new_path.empty())
                                        {
                                            path2.path = new_path;
                                            path2.current_node_index = 0;
                                            path2.start_node = start_node;
                                        }

                                        new_target_node->walkable = target_was_walkable;
                                        start_node->walkable = start_was_walkable;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

sf::Vector2f SquadMovementSystem::getSquadCenter(entt::registry& registry, uint32_t squad_id)
{
    auto members = getSquadMembers(registry, squad_id);
    if (members.empty()) return sf::Vector2f(0, 0);

    sf::Vector2f center(0, 0);
    for (auto member : members)
    {
        auto& pos = registry.get<Components::Position>(member);
        center.x += pos.position.x;
        center.y += pos.position.y;
    }

    center.x /= static_cast<float>(members.size());
    center.y /= static_cast<float>(members.size());

    return center;
}
bool SquadMovementSystem::checkSquadCollision(entt::registry& registry, uint32_t squad1_id, uint32_t squad2_id)
{
    sf::Vector2f center1 = getSquadCenter(registry, squad1_id);
    sf::Vector2f center2 = getSquadCenter(registry, squad2_id);

    sf::Vector2f delta = center1 - center2;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    // Более разумное расстояние столкновения
    float min_distance = 80.0f;

    return distance < min_distance;
}
entt::entity SquadMovementSystem::getSquadLeader(entt::registry& registry, uint32_t squad_id)
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

    for (auto entity : squad_view)
    {
        auto& squad = registry.get<Components::Squad>(entity);
        if (squad.squad_id == squad_id && squad.formation_index == 0)
        {
            return entity;
        }
    }

    return entt::null;
}
std::vector<entt::entity> SquadMovementSystem::getSquadMembers(entt::registry& registry, uint32_t squad_id)
{
    std::vector<entt::entity> members;
    auto squad_view = registry.view<Components::Squad>();

    for (auto entity : squad_view)
    {
        auto& squad = registry.get<Components::Squad>(entity);
        if (squad.squad_id == squad_id)
        {
            members.push_back(entity);
        }
    }

    return members;
}