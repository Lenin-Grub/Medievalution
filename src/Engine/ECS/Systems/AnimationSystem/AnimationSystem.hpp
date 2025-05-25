#pragma once
#include "../ISystem.hpp"

class AnimationSystem
{
public:
    static void update(entt::registry& registry, float deltaTime);
};
