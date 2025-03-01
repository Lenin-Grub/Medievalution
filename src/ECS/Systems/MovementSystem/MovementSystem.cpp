#include "../../../stdafx.h"
#include  "MovementSystem.hpp"

void MovementSystem::update(entt::registry& registry, float deltaTime)
{
    auto view = registry.view<Component_Position, Component_Velocity>();
    for (auto entity : view)
    {
        auto& position       = view.get<Component_Position>(entity);
        const auto& velocity = view.get<Component_Velocity>(entity);
        position.position   += velocity.velocity * deltaTime;
    }
}