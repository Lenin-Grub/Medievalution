#include "../../../stdafx.h"
#include "HandleInputSystem.hpp"

void HandleInputSystem::update(entt::registry& registry)
{
    auto view = registry.view<Control>();
    for (auto entity : view)
    {
        auto& control = view.get<Control>(entity);

        control.direction = sf::Vector2f(0.0f, 0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            control.direction.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            control.direction.y += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            control.direction.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            control.direction.x += 1.0f;
    }
}