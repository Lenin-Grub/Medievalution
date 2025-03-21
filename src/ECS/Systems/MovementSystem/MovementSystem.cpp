#include "../../../stdafx.h"
#include  "MovementSystem.hpp"

void MovementSystem::update(entt::registry& registry, float deltaTime)
{
    auto view = registry.view<Components::Position, Components::Velocity>();
    for (auto entity : view)
    {
        auto& position       = view.get<Components::Position>(entity);
        const auto& velocity = view.get<Components::Velocity>(entity);
        position.position   += velocity.velocity * deltaTime;
    }
}