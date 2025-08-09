#pragma once
#include "../ISystem.hpp"
#include "../../../Pathfinding/Pathfinding.h"
#include <vector>

enum class FormationType
{
    Line,       // Линия
    Square,     // Квадрат
    Circle,     // Круг
    Diamond,    // Ромб
    Triangle    // Треугольник (клином)
};

class SquadMovementSystem
{
public:
    static void update(entt::registry& registry, Pathfinding& global_pathfinding, float delta_time);

private:
    static void updateSquadFormation(entt::registry& registry, Pathfinding& global_pathfinding);
    static entt::entity getSquadLeader(entt::registry& registry, uint32_t squad_id);
    static std::vector<entt::entity> getSquadMembers(entt::registry& registry, uint32_t squad_id);
    static sf::Vector2f calculateFormationOffset(uint32_t formation_index, const sf::Vector2f& move_direction, float spacing, Components::FormationType formation_type);
    static sf::Vector2f calculateLineOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing);
    static sf::Vector2f calculateSquareOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing);
    static sf::Vector2f calculateCircleOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing);
    static sf::Vector2f calculateDiamondOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing);
    static sf::Vector2f calculateTriangleOffset(uint32_t formation_index, const sf::Vector2f& forward, const sf::Vector2f& right, float spacing);
    static sf::Vector2f getSquadCenter(entt::registry& registry, uint32_t squad_id);
    static bool checkSquadCollision(entt::registry& registry, uint32_t squad1_id, uint32_t squad2_id);
    static void avoidSquadCollisions(entt::registry& registry, Pathfinding& global_pathfinding);
};