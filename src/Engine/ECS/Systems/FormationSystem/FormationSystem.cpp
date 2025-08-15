#include "FormationSystem.hpp"
#include <algorithm>
#include <numbers>

#pragma region Main Methods

void SquadMovementSystem::update(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time)
{
    global_pathfinding.resetWalkable();

    auto position_view = registry.view<Components::Position>();
    for (auto entity : position_view) 
    {
        const auto& pos = registry.get<Components::Position>(entity);
        Node* node = global_pathfinding.getNodeByPosition(pos.position);
        if (node)
        {
            node->walkable = false;
        }
    }

    // with Squad component
    updateSquadFormation(registry, global_pathfinding);
    // withot Squad component
    updateMovement(registry, global_pathfinding, delta_time);
}

std::vector<entt::entity> SquadMovementSystem::getSquadMembers(entt::registry& registry, uint32_t squad_id)
{
    std::vector<entt::entity> members;
    auto squad_view = registry.view<Components::Formation>();

    for (auto entity : squad_view)
    {
        const auto& squad = registry.get<Components::Formation>(entity);
        if (squad.squad_id == squad_id)
            members.push_back(entity);
    }

    return members;
}

entt::entity SquadMovementSystem::getSquadLeader(entt::registry& registry, uint32_t squad_id)
{
    auto squad_view = registry.view<Components::Formation>();

    for (auto entity : squad_view)
    {
        const auto& squad = registry.get<Components::Formation>(entity);

        if (squad.squad_id == squad_id && squad.is_leader)
            return entity;
    }

    for (auto entity : squad_view)
    {
        const auto& squad = registry.get<Components::Formation>(entity);
        if (squad.squad_id == squad_id && squad.formation_index == 0)
            return entity;
    }

    return entt::null;
}

sf::Vector2f SquadMovementSystem::getSquadCenter(entt::registry& registry, uint32_t squad_id)
{
    auto members = getSquadMembers(registry, squad_id);
    if (members.empty()) return sf::Vector2f(0, 0);

    sf::Vector2f center(0, 0);
    for (auto member : members)
    {
        const auto& pos = registry.get<Components::Position>(member);
        center += pos.position;
    }

    return center / static_cast<float>(members.size());
}

#pragma endregion

#pragma region Updates & Hadles

void SquadMovementSystem::updateSquadFormation(entt::registry& registry, Pathfinding& global_pathfinding)
{
    std::unordered_set<uint32_t> processed_squads;
    auto squad_view = registry.view<Components::Formation>();

    for (auto entity : squad_view)
    {
        const auto& squad = registry.get<Components::Formation>(entity);

        if (squad.squad_id == 0 || !processed_squads.emplace(squad.squad_id).second)
            continue;

        processSquadFormation(registry, global_pathfinding, squad.squad_id);
    }
}

void SquadMovementSystem::processSquadFormation(entt::registry& registry, Pathfinding& global_pathfinding, uint32_t squad_id)
{
    entt::entity leader = getSquadLeader(registry, squad_id);
    if (leader == entt::null || !registry.valid(leader))
        return;

    const auto& leader_path = registry.get<Components::Pathfinding>(leader);
    const auto& leader_pos = registry.get<Components::Position>(leader);

    if (!isSquadSelected(registry, leader))
        return;

    if (!leader_path.end_node || leader_path.path.empty())
        return;

    updateSquadMembersFormation(registry, global_pathfinding, squad_id, leader, leader_path, leader_pos);
}

bool SquadMovementSystem::isSquadSelected(entt::registry& registry, entt::entity leader)
{
    if (auto* selectable = registry.try_get<Components::Selectable>(leader))
    {
        return selectable->is_selected;
    }
    return false;
}

void SquadMovementSystem::updateSquadMembersFormation(entt::registry& registry, Pathfinding& global_pathfinding, uint32_t squad_id, entt::entity leader, const Components::Pathfinding& leader_path, const Components::Position& leader_pos)
{
    auto members = getSquadMembers(registry, squad_id);
    members.erase(std::remove(members.begin(), members.end(), leader), members.end());

    sf::Vector2f leader_target = leader_path.end_node->position;
    sf::Vector2f move_direction = leader_target - leader_pos.position;

    for (auto member : members)
    {
        updateMemberFormation(registry, global_pathfinding, member, leader_target, move_direction);
    }
}

void SquadMovementSystem::updateMemberFormation(entt::registry& registry, Pathfinding& global_pathfinding, entt::entity member, const sf::Vector2f& leader_target, const sf::Vector2f& move_direction)
{
    const auto& member_squad = registry.get<Components::Formation>(member);
    auto& member_path = registry.get<Components::Pathfinding>(member);
    const auto& member_pos = registry.get<Components::Position>(member);

    sf::Vector2f formation_offset = calculateFormationOffset(member_squad.formation_index, move_direction,
        DEFAULT_SPACING, member_squad.formation_type);
    sf::Vector2f formation_target = leader_target + formation_offset;

    if (!shouldUpdateFormationPath(member_path, formation_target, DEFAULT_SPACING))
        return;

    updateMemberPath(registry, global_pathfinding, member, member_path, member_pos, formation_target);
}

void SquadMovementSystem::updateMemberPath(entt::registry& registry, Pathfinding& global_pathfinding, entt::entity member, Components::Pathfinding& member_path, const Components::Position& member_pos, const sf::Vector2f& formation_target)
{
    Node* target_node = global_pathfinding.getNodeByPosition(formation_target);
    Node* start_node = global_pathfinding.getNodeByPosition(member_pos.position);

    if (!target_node || !start_node || start_node == target_node)
        return;

    updatePathWithTemporaryWalkable(registry, global_pathfinding, member, member_path,
        start_node, target_node, member_pos);
}

void SquadMovementSystem::updatePathWithTemporaryWalkable(entt::registry& registry, Pathfinding& global_pathfinding, entt::entity member, Components::Pathfinding& member_path, Node* start_node, Node* target_node, const Components::Position& member_pos)
{
    bool target_was_walkable = target_node->walkable;
    bool start_was_walkable = start_node->walkable;

    target_node->walkable = true;
    start_node->walkable = true;

    global_pathfinding.findPath(start_node, target_node);

    auto new_path = global_pathfinding.path();
    if (!new_path.empty())
    {
        member_path.path = std::move(new_path);
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

#pragma endregion

#pragma region Formation

sf::Vector2f SquadMovementSystem::calculateFormationOffset(uint32_t formation_index, const sf::Vector2f& move_direction, float spacing, Components::FormationType formation_type) 
{
    sf::Vector2f normalized_dir = Math::normalizeVector(move_direction);
    sf::Vector2f perp_dir = Math::perpendicular(normalized_dir);

    switch (formation_type) 
    {
    case Components::FormationType::Line:
        return calculateLineOffset(formation_index, normalized_dir, perp_dir, spacing);
    case Components::FormationType::Square:
        return calculateSquareOffset(formation_index, normalized_dir, perp_dir, spacing);
    case Components::FormationType::Circle:
        return calculateCircleOffset(formation_index, normalized_dir, perp_dir, spacing);
    default:
        return calculateLineOffset(formation_index, normalized_dir, perp_dir, spacing);
    }
}

sf::Vector2f SquadMovementSystem::calculateLineOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing)
{
    /*
     1 = leader, 0 = soldier
     Foramtion looks like:

                    1
                000000000

    */

    if (formation_index == 0)
        return forward * spacing;
    else
    {
        int line_pos = static_cast<int>(formation_index) - 5;
        sf::Vector2f neg_forward(-forward.x * spacing, -forward.y * spacing);
        sf::Vector2f right_scaled(right.x * line_pos * spacing, right.y * line_pos * spacing);
        return sf::Vector2f(neg_forward.x + right_scaled.x, neg_forward.y + right_scaled.y);
    }
}

sf::Vector2f SquadMovementSystem::calculateSquareOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing) 
{
    /*
     1 = leader, 0 = soldier
     Foramtion looks like

                   000
                 1 000
                   000

    */

    if (formation_index == 0) 
    {
        return forward * spacing;
    }
    else 
    {
        int index_in_square = static_cast<int>(formation_index) - 1;

        //TODO redo it
        // Square 3x3 
        int row = index_in_square / 3;
        int col = index_in_square % 3;

        float local_x = static_cast<float>(col - 1);  // -1, 0, 1
        float local_y = static_cast<float>(row - 1);  // -1, 0, 1

        float world_x = local_x * right.x + local_y * forward.x;
        float world_y = local_x * right.y + local_y * forward.y;

        world_x *= spacing;
        world_y *= spacing;

        // Behind leader
        float behind_x = forward.x * (-spacing * 2.0f);
        float behind_y = forward.y * (-spacing * 2.0f);

        return sf::Vector2f(world_x + behind_x, world_y + behind_y);
    }
}

sf::Vector2f SquadMovementSystem::calculateCircleOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing) 
{
    // leader in centre of circle

    if (formation_index == 0) 
    {
        return sf::Vector2f(0, 0);
    }
    else 
    {
        int units_in_circle = 9;
        float angle_step = 2.0f * std::numbers::pi / units_in_circle;
        float angle = (static_cast<int>(formation_index) - 1) * angle_step;
        float radius = spacing * 2.0f;

        return sf::Vector2f(right.x * radius * std::cos(angle) - forward.x * radius * std::sin(angle),
                            right.y * radius * std::cos(angle) - forward.y * radius * std::sin(angle));
    }
}

#pragma endregion

#pragma region Pathfinding

bool SquadMovementSystem::shouldUpdateFormationPath(const Components::Pathfinding& path_comp, const sf::Vector2f& target_position, float threshold)
{
    if (path_comp.path.empty() || !path_comp.end_node) 
    {
        return true;
    }

    sf::Vector2f current_target = path_comp.end_node->position;
    sf::Vector2f delta = target_position - current_target;
    float distance = Math::length(delta);

    return distance > threshold * 0.5f;
}

void SquadMovementSystem::updateMovement(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time) 
{
    auto view = registry.view<Components::Pathfinding, Components::Position, Components::Velocity>();

    for (auto entity : view) 
    {
        auto& path_comp = registry.get<Components::Pathfinding>(entity);
        auto& pos_comp  = registry.get<Components::Position>(entity);
        auto& vel_comp  = registry.get<Components::Velocity>(entity);

        if (path_comp.path.empty() || path_comp.current_node_index >= path_comp.path.size()) 
        {
            if (auto* state = registry.try_get<Components::State>(entity)) 
            {
                if (state->state == Components::CharacterState::Move)
                    state->state = Components::CharacterState::Idle;
            }
            continue;
        }

        Node* next_node = path_comp.path[path_comp.current_node_index];
        sf::Vector2f direction = next_node->position - pos_comp.position;
        float distance = Math::length(direction);

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

#pragma endregion