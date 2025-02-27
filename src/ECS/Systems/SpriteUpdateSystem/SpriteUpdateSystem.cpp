#include "../../../stdafx.h"
#include "SpriteUpdateSystem.hpp"

void SpriteUpdateSystem::update(entt::registry& registry, Animator animator)
{
    auto spriteView = registry.view<Component_Position, Component_Sprite>();
    for (auto entity : spriteView)
    {
        auto& sprite = spriteView.get<Component_Sprite>(entity);
        const auto& position = spriteView.get<Component_Position>(entity);

        sprite.sprite.setPosition(position.position);
        sprite.sprite.setTextureRect(animator.getCurrentFrameRect());
    }
}
