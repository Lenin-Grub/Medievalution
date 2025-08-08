#include "StateSystem.hpp"

#include "StateSystem.hpp"

void StateSystem::update(entt::registry& registry, float delta_time)
{
    auto view = registry.view<Components::State, Components::Animation, Components::Position>(entt::exclude<Components::Pathfinding>);
    auto path_view = registry.view<Components::State, Components::Animation, Components::Pathfinding, Components::Position>();

    static std::unordered_map<entt::entity, Components::CharacterState> last_state;
    static std::unordered_map<entt::entity, std::string> last_animation;
    static std::unordered_map<entt::entity, std::string> last_direction;

    // No Pathfinding
    for (auto entity : view)
    {
        auto& state = registry.get<Components::State>(entity);
        auto& animation = registry.get<Components::Animation>(entity);
        auto& pos_comp = registry.get<Components::Position>(entity);

        std::string anim_name;
        bool loop = true;
        std::string direction = "south";

        if (auto* vel = registry.try_get<Components::Velocity>(entity))
        {
            direction = getDirectionFromOffset(vel->velocity);
        }

        if (state.state == Components::CharacterState::Move)
        {
            anim_name = "run_" + direction;
        }
        else if (state.state == Components::CharacterState::Attack)
        {
            anim_name = "attack_" + direction;
            loop = false;
        }
        else if (state.state == Components::CharacterState::Idle)
        {
            anim_name = "idle_" + direction;
        }
        else
        {
            anim_name = "idle_south"; // or last_animation
        }

        bool state_changed = last_state[entity] != state.state;
        bool direction_changed = last_direction[entity] != direction;
        bool should_update = state_changed || direction_changed;

        if (!anim_name.empty() && should_update)
        {
            animation.animator.setAnimation(anim_name, loop);

            last_animation[entity] = anim_name;
            last_state[entity]     = state.state;
            last_direction[entity] = direction;
        }
    }

    // With Pathfinding
    for (auto entity : path_view)
    {
        auto& state     = registry.get<Components::State>(entity);
        auto& animation = registry.get<Components::Animation>(entity);
        auto& path_comp = registry.get<Components::Pathfinding>(entity);
        auto& pos_comp  = registry.get<Components::Position>(entity);

        std::string anim_name;
        bool loop = true;
        std::string direction = "south";

        if (state.state == Components::CharacterState::Move && !path_comp.path.empty() &&
            path_comp.current_node_index < path_comp.path.size())
        {
            Node* next_node = path_comp.path[path_comp.current_node_index];
            sf::Vector2f offset = next_node->position - pos_comp.position;
            direction = getDirectionFromOffset(offset);
            anim_name = "run_" + direction;
        }
        else if (state.state == Components::CharacterState::Attack)
        {
            auto it = last_direction.find(entity);
            direction = (it != last_direction.end()) ? it->second : "south";
            anim_name = "attack_" + direction;
            loop = false;
        }
        else if (state.state == Components::CharacterState::Idle)
        {
            auto it = last_direction.find(entity);
            direction = (it != last_direction.end()) ? it->second : "south";
            anim_name = "idle_" + direction;
        }

        bool state_changed     = last_state[entity] != state.state;
        bool direction_changed = last_direction[entity] != direction;
        bool should_update     = state_changed || direction_changed;

        if (!anim_name.empty() && should_update)
        {
            animation.animator.setAnimation(anim_name, loop);
            last_animation[entity] = anim_name;
            last_state[entity]     = state.state;
            last_direction[entity] = direction;
        }
    }
}

std::string StateSystem::getDirectionFromOffset(const sf::Vector2f& offset)
{
    float x = offset.x;
    float y = offset.y;

    // Normalize
    int dx = (std::abs(x) > 0.1f) ? (x > 0 ? 1 : -1) : 0;
    int dy = (std::abs(y) > 0.1f) ? (y > 0 ? 1 : -1) : 0;

    if (dx == 1  && dy == 0)  return "east";
    if (dx == -1 && dy == 0)  return "west";
    if (dx == 0  && dy == 1)  return "south";
    if (dx == 0  && dy == -1) return "north";
    if (dx == 1  && dy == -1) return "north_east";
    if (dx == -1 && dy == -1) return "north_west";
    if (dx == 1  && dy == 1)  return "south_east";
    if (dx == -1 && dy == 1)  return "south_west";

    return "south"; // default
}