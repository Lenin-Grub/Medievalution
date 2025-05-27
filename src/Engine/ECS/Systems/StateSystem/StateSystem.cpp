#include "StateSystem.hpp"

void StateSystem::update(entt::registry& registry, float delta_time)
{
    auto view = registry.view<Components::State, Components::Animation>();

    for (auto entity : view)
    {
        auto& registry_ref = registry;
        auto& state        = registry_ref.get<Components::State>    (entity);
        auto& animation    = registry_ref.get<Components::Animation>(entity);

        static std::unordered_map<entt::entity, Components::CharacterState> last_state;

        if (last_state[entity] == state.state)
            continue;

        switch (state.state)
        {
        case Components::CharacterState::Idle:
            animation.animator.setAnimation("stand", true);
            break;

        case Components::CharacterState::Move:
            animation.animator.setAnimation("run", true);
            break;

        case Components::CharacterState::Attack:
            animation.animator.setAnimation("attack", false);
            break;

        case Components::CharacterState::Dead:
            animation.animator.setAnimation("dead", false);
            break;

        default:
            animation.animator.setAnimation("stand", true);
            break;
        }

        last_state[entity] = state.state;
    }
}