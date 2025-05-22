#include "../../../stdafx.h"
#include "SpriteUpdateSystem.hpp"

void SpriteSystem::update(entt::registry& registry, std::optional<Animator> animator)
{
    auto sprite_view = registry.view<Components::Position, Components::Sprite>();
    for (auto entity : sprite_view)
    {
        auto& sprite         = sprite_view.get<Components::Sprite>  (entity);
        const auto& position = sprite_view.get<Components::Position>(entity);

        sprite.sprite.setPosition(position.position);
    }
}
