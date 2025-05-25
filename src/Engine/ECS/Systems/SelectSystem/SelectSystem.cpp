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

            auto view = registry.view<Components::Selectable>();
            for (auto entity : view) 
            {
                auto& selectable = view.get<Components::Selectable>(entity);
                selectable.is_selected = false;
            }
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

    auto view = registry.view<Components::Sprite, Components::Position, Components::Selectable>();
    for (auto entity : view) 
    {
        auto& sprite_component      = view.get<Components::Sprite>    (entity);
        auto& position_component    = view.get<Components::Position>  (entity);
        auto& selectable_component  = view.get<Components::Selectable>(entity);

        sf::FloatRect entity_bounds = sprite_component.sprite.getGlobalBounds();
        entity_bounds.left          = position_component.position.x - 8;
        entity_bounds.top           = position_component.position.y - 32;

        if (selection_rect.intersects(entity_bounds)) 
            selectable_component.is_selected = true;
        else if (entity_bounds.contains(common::mouse_pos_view))
            selectable_component.is_selected = true;
    }
}