#pragma once
#include "../ISystem.hpp"

class MovementSystem
{
public:
    static void update(entt::registry& registry, float deltaTime);
};