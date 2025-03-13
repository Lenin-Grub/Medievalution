#include "../../../stdafx.h"
#include "ControlSystem.hpp"

void ControlSystem::update(entt::registry& registry)
{
    auto view = registry.view<Component_Velocity, Component_Control>();
    for (auto entity : view)
    {
        auto& velocity      = view.get<Component_Velocity>(entity);
        const auto& control = view.get<Component_Control>(entity);

        velocity.velocity = control.direction * velocity.speed;
    }
}