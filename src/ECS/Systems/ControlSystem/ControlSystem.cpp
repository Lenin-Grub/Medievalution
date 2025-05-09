#include "../../../stdafx.h"
#include "ControlSystem.hpp"

void ControlSystem::update(entt::registry& registry)
{
    auto view = registry.view<Components::Velocity, Components::Control>();
    for (auto [entity, velocity, control] : view.each())
    {
        velocity.velocity = control.direction * velocity.speed;
    }
}