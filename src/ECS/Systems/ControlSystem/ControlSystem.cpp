#include "../../../stdafx.h"
#include "ControlSystem.hpp"

void ControlSystem::update(entt::registry& registry)
{
    auto view = registry.view<Component_Velocity, Control>();
    for (auto entity : view)
    {
        auto& velocity      = view.get<Component_Velocity>(entity);
        const auto& control = view.get<Control>(entity);

        float speed = 100.0f;

        velocity.velocity = control.direction * speed;
    }
}