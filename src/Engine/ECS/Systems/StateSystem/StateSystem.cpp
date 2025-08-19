#include "StateSystem.hpp"

std::unordered_map<entt::entity, EntityStateCache> StateSystem::entity_cache;

void StateSystem::update(entt::registry& registry, float delta_time) 
{
    auto view = registry.view<Components::State, Components::Animation, Components::Position>();
    auto path_view = registry.view<Components::State, Components::Animation, Components::Pathfinding, Components::Position>();

    // With Pathfinding
    for (auto entity : path_view) 
    {
        updateEntity(registry, entity, true);
    }

    // Whitout Pathfinding
    for (auto entity : view) 
    {
        if (!registry.all_of<Components::Pathfinding>(entity)) 
        {
            updateEntity(registry, entity, false);
        }
    }

    for (auto it = entity_cache.begin(); it != entity_cache.end();) 
    {
        if (!registry.valid(it->first))
            it = entity_cache.erase(it);
        else
            ++it;
    }
}

void StateSystem::updateEntity(entt::registry& registry, entt::entity entity, bool has_pathfinding) 
{
    auto& state     = registry.get<Components::State>(entity);
    auto& animation = registry.get<Components::Animation>(entity);
    auto& pos_comp  = registry.get<Components::Position>(entity);

    std::string direction = "south";
    bool loop = true;

    if (has_pathfinding) 
    {
        auto& path_comp = registry.get<Components::Pathfinding>(entity);
        if (state.state == Components::CharacterState::Run && !path_comp.path.empty() && path_comp.current_node_index < path_comp.path.size()) 
        {
            Node* next_node = path_comp.path[path_comp.current_node_index];
            sf::Vector2f offset = next_node->position - pos_comp.position;
            direction = getDirectionFromOffset(offset);
        }
        else 
        {
            auto it = entity_cache.find(entity);
            if (it != entity_cache.end()) 
                direction = it->second.last_direction;
        }
    }
    else 
    {
        if (auto* vel = registry.try_get<Components::Velocity>(entity))
            direction = getDirectionFromOffset(vel->velocity);
    }

    bool loop_out;
    std::string anim_name = getAnimationName(state.state, direction, loop_out);

    auto& cache = entity_cache[entity];
    bool state_changed = cache.last_state != state.state;
    bool direction_changed = cache.last_direction != direction;
    bool should_update = state_changed || direction_changed || cache.last_animation != anim_name;

    if (should_update) 
    {
        animation.animator.setAnimation(anim_name, loop_out);
        cache.last_state = state.state;
        cache.last_direction = direction;
        cache.last_animation = anim_name;
    }
}

std::string StateSystem::getAnimationName(Components::CharacterState state, const std::string& direction, bool& loop) 
{
    loop = true;

    switch (state) 
    {
    case Components::CharacterState::Idle:
        return "idle_" + direction;
    case Components::CharacterState::Run:
        return "run_" + direction;
    case Components::CharacterState::Walk:
        return "walk_" + direction;
    case Components::CharacterState::Taunt:
        return "taunt_" + direction;
    case Components::CharacterState::Dead:
        loop = false;
        return "dead_" + direction;
    case Components::CharacterState::Hit:
        loop = false;
        return "hit_" + direction;
    case Components::CharacterState::Attack:
        loop = false;
        return "attack_" + direction;
    case Components::CharacterState::Shoot:
        loop = false;
        return "shoot_" + direction;
    default:
        return "idle_south";
    }
}

std::string StateSystem::getDirectionFromOffset(const sf::Vector2f& offset) 
{
    float x = offset.x;
    float y = offset.y;

    int dx = (std::abs(x) > 0.1f) ? (x > 0 ? 1 : -1) : 0;
    int dy = (std::abs(y) > 0.1f) ? (y > 0 ? 1 : -1) : 0;

    if (dx == 1 && dy == 0) return "east";
    if (dx == -1 && dy == 0) return "west";
    if (dx == 0 && dy == 1) return "south";
    if (dx == 0 && dy == -1) return "north";
    if (dx == 1 && dy == -1) return "north_east";
    if (dx == -1 && dy == -1) return "north_west";
    if (dx == 1 && dy == 1) return "south_east";
    if (dx == -1 && dy == 1) return "south_west";

    return "south";
}