#include "../../../stdafx.h"
#include "SelectSystem.hpp"


void SelectSystem::update(entt::registry& registry)
{
    static sf::Vector2f start_selection;
    static bool is_selecting = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!is_selecting)
        {
            start_selection = common::mouse_pos_view;
            is_selecting = true;
        }
    }
    else
    {
        if (is_selecting)
        {
            is_selecting = false;
            selectRectangle(registry, start_selection, common::mouse_pos_view);
        }
    }
}

void SelectSystem::selectRectangle(entt::registry& registry, const sf::Vector2f& start, const sf::Vector2f& end)
{
    sf::FloatRect selection_rect(start, end - start);

    auto view = registry.view<Component_Sprite, Component_Position>();
    for (auto entity : view)
    {
        auto& sprite_component = view.get<Component_Sprite>(entity);
        auto& position_component = view.get<Component_Position>(entity);
        sf::FloatRect entity_bounds = sprite_component.sprite.getGlobalBounds();
        entity_bounds.left = position_component.position.x;
        entity_bounds.top = position_component.position.y;

        if (selection_rect.intersects(entity_bounds))
            sprite_component.sprite.setColor(sf::Color::Red);
        else
            sprite_component.sprite.setColor(sf::Color::White);
    }
}