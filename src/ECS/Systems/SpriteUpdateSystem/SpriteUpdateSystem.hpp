#pragma once
#include "../ISystem.hpp"
#include "../../../Animation/Animation.hpp"

class SpriteUpdateSystem
{
public:
    static void update(entt::registry& registry, Animator animator);
};