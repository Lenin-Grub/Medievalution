#include "../../../stdafx.h"
#include "SpriteUpdateSystem.hpp"

void SpriteUpdateSystem::update(entt::registry& registry, Animator animator)
{
    auto sprite_view = registry.view<Component_Position, Component_Sprite>();
    for (auto entity : sprite_view)
    {
        auto& sprite         = sprite_view.get<Component_Sprite>(entity);
        const auto& position = sprite_view.get<Component_Position>(entity);

        sprite.sprite.setPosition(position.position);
        sprite.sprite.setTextureRect(animator.getCurrentFrameRect());
    }
}
