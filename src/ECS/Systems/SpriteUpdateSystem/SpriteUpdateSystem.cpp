#include "../../../stdafx.h"
#include "SpriteUpdateSystem.hpp"

void SpriteUpdateSystem::update(entt::registry& registry, Animator animator)
{
    auto sprite_view = registry.view<Components::Position, Components::Sprite>();
    for (auto entity : sprite_view)
    {
        auto& sprite         = sprite_view.get<Components::Sprite>(entity);
        const auto& position = sprite_view.get<Components::Position>(entity);

        sprite.sprite.setPosition(position.position);
        sprite.sprite.setTextureRect(animator.getCurrentFrameRect());
    }
}

void SpriteUpdateSystem::update(entt::registry& registry)
{
    auto sprite_view = registry.view<Components::Position, Components::Sprite>();
    for (auto entity : sprite_view)
    {
        auto& sprite = sprite_view.get<Components::Sprite>(entity);
        const auto& position = sprite_view.get<Components::Position>(entity);

        sprite.sprite.setPosition(position.position);
    }
}
