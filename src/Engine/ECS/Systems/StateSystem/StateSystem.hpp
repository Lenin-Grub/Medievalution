#pragma once
#include "../ISystem.hpp"

class StateSystem
{
public:
    static void update(entt::registry& registry, float dt);
    static std::string getDirectionFromOffset(const sf::Vector2f& offset);
};