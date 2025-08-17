#include "SelectSystem.hpp"
#include <imgui.h>

void SelectSystem::update(entt::registry& registry, const sf::Vector2f& mouse_pos)
{
    static sf::Vector2f start_selection;
    static bool is_selecting = false;

    const bool is_left_mouse_down = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    // if (ImGui::GetIO().WantCaptureMouse)
    //     return;

    if (is_left_mouse_down && !is_selecting)
    {
        start_selection = mouse_pos;
        is_selecting = true;

        auto view = registry.view<Components::Selectable>();
        for (auto entity : view)
        {
            auto& selectable = view.get<Components::Selectable>(entity);
            selectable.is_selected = false;
        }
    }

    if (!is_left_mouse_down && is_selecting)
    {
        is_selecting = false;
        selectRectangle(registry, start_selection, mouse_pos);
    }
}

void SelectSystem::selectRectangle(entt::registry& registry, const sf::Vector2f& start, const sf::Vector2f& end)
{
    sf::FloatRect selection_rect(start, end - start);

    auto view = registry.view<Components::Sprite, Components::Position, Components::Selectable>();
    for (auto entity : view)
    {
        auto& sprite_component     = view.get<Components::Sprite>(entity);
        auto& position_component   = view.get<Components::Position>(entity);
        auto& selectable_component = view.get<Components::Selectable>(entity);

        sf::FloatRect entity_bounds;
        entity_bounds.left = position_component.position.x;
        entity_bounds.top  = position_component.position.y;
        
        sf::FloatRect sprite_bounds = sprite_component.sprite.getGlobalBounds();
        entity_bounds =  sprite_bounds;

        entity_bounds.left -= entity_bounds.width / 2;
        entity_bounds.top  -= entity_bounds.height / 2;

        if (selection_rect.intersects(entity_bounds))
        {
            selectable_component.is_selected = true;
        }
    }
}
