#pragma once
#include "../ISystem.hpp"
#include "../../../Pathfinding/Pathfinding.h"
#include <vector>
#include <Math/Math.h>

class SquadMovementSystem 
{
public:
    static void update(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time);

    static std::vector<entt::entity> getSquadMembers(entt::registry& registry, uint32_t squad_id);
    static entt::entity getSquadLeader(entt::registry& registry, uint32_t squad_id);
    static sf::Vector2f getSquadCenter(entt::registry& registry, uint32_t squad_id);

private:
    static void updateSquadFormation(entt::registry& registry, Pathfinding& global_pathfinding);

private:
    static void processSquadFormation(entt::registry& registry, Pathfinding& global_pathfinding, uint32_t squad_id);
    static bool isSquadSelected(entt::registry& registry, entt::entity leader);
    static void updateSquadMembersFormation(entt::registry& registry, Pathfinding& global_pathfinding, uint32_t squad_id, entt::entity leader, const Components::Pathfinding& leader_path, const Components::Position& leader_pos);
    static void updateMemberFormation(entt::registry& registry, Pathfinding& global_pathfinding, entt::entity member, const sf::Vector2f& leader_target, const sf::Vector2f& move_direction);
    static void updateMemberPath(entt::registry& registry, Pathfinding& global_pathfinding,entt::entity member, Components::Pathfinding& member_path, const Components::Position& member_pos,const sf::Vector2f& formation_target);
    static void updatePathWithTemporaryWalkable(entt::registry& registry, Pathfinding& global_pathfinding, entt::entity member, Components::Pathfinding& member_path, Node* start_node, Node* target_node, const Components::Position& member_pos);

private:
    static sf::Vector2f calculateFormationOffset(uint32_t formation_index, const sf::Vector2f& move_direction, float spacing, Components::FormationType formation_type);
    static sf::Vector2f calculateLineOffset  (uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing);
    static sf::Vector2f calculateSquareOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing);
    static sf::Vector2f calculateCircleOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing);

    static bool shouldUpdateFormationPath(const Components::Pathfinding& path_comp, const sf::Vector2f& target_position, float threshold);

    static void updateMovement(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time);

private:
    static constexpr float DEFAULT_SPACING        = 32.0f;
};