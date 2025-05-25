#include "StateSystem.hpp"

void StateSystem::update(entt::registry& registry, float delta_time)
{
    auto view = registry.view<Components::State, Components::Animation>();

    for (auto entity : view)
    {
        auto& registryRef = registry;
        auto& state = registryRef.get<Components::State>(entity);
        auto& animation = registryRef.get<Components::Animation>(entity);

        static std::unordered_map<entt::entity, Components::CharacterState> last_state;

        if (last_state[entity] == state.state)
            continue;

        LOG_INFO("Changing state to: {}", static_cast<int>(state.state));

        switch (state.state)
        {
        case Components::CharacterState::Idle:
            LOG_INFO("Setting animation: stand");
            animation.animator.setAnimation("stand", true);
            break;

        case Components::CharacterState::Move:
            LOG_INFO("Setting animation: run");
            animation.animator.setAnimation("run", true);
            break;

        case Components::CharacterState::Attack:
            LOG_INFO("Setting animation: attack");
            animation.animator.setAnimation("attack", false);
            break;

        case Components::CharacterState::Dead:
            LOG_INFO("Setting animation: dead");
            animation.animator.setAnimation("dead", false);
            break;
        }

        last_state[entity] = state.state;
    }
}