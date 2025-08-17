#pragma once
#include "../ISystem.hpp"

class SelectSystem
{
public:
    static void update(entt::registry& registry, const sf::Vector2f& mouse_pos);

private:
    static void selectRectangle(entt::registry& registry, const sf::Vector2f& start, const sf::Vector2f& end);
};
