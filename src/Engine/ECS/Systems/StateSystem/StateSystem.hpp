#pragma once
#include "../ISystem.hpp"

struct EntityStateCache
{
    Components::CharacterState last_state = Components::CharacterState::Idle;
    std::string last_animation;
    std::string last_direction = "south";
};

class StateSystem 
{
public:
    static void update(entt::registry& registry, float delta_time);

private:

    static std::unordered_map<entt::entity, EntityStateCache> entity_cache;

    static void updateEntity(entt::registry& registry, entt::entity entity, bool has_pathfinding);
    static std::string getAnimationName(Components::CharacterState state, const std::string& direction, bool& loop);
    static std::string getDirectionFromOffset(const sf::Vector2f& offset);
};
