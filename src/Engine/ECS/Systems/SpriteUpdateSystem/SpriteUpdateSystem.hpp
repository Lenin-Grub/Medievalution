#pragma once
#include "../ISystem.hpp"
#include <Animation/Animator/Animator.hpp>

class SpriteSystem
{
public:
    static void update(entt::registry& registry, std::optional <Animator> animator);
};