#include "../../../stdafx.h"
#include  "AnimationSystem.hpp"

void AnimationSystem::update(entt::registry& registry, float delta_time)
{
    registry.view<Components::Animation>().each(
        [&](Components::Animation& animatorComp)
        {
            animatorComp.animator.update(delta_time);
        });
}
