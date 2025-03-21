#include "../../../stdafx.h"
#include "ControlSystem.hpp"

void ControlSystem::update(entt::registry& registry)
{
    auto view = registry.view<Components::Velocity, Components::Control>();
    for (auto entity : view)
    {
        auto& velocity      = view.get<Components::Velocity>(entity);
        const auto& control = view.get<Components::Control>(entity);

        velocity.velocity = control.direction * velocity.speed;
    }
}