#include "../../../stdafx.h"
#include  "RenderSystem.hpp"

void RenderSystem::render(entt::registry& registry, sf::RenderWindow& window)
{
    auto view = registry.view<Components::Sprite, Components::Position>();

    for (auto entity : view)
    {
        auto& sprite_component     = view.get<Components::Sprite>     (entity);
        auto& position_component   = view.get<Components::Position>   (entity);

        // Draw entityes
        window.draw(sprite_component.sprite);
        selectionBox(window);
    }
}

void RenderSystem::selectionBox(sf::RenderWindow& window)
{
    static sf::Vector2f startSelection;
    static bool         isSelecting = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!isSelecting)
        {
            startSelection = common::mouse_pos_view;
            isSelecting = true;
        }
    }
    else
    {
        if (isSelecting)
            isSelecting = false;
    }

    if (isSelecting)
    {
        sf::RectangleShape selectionBox;
        selectionBox.setFillColor(sf::Color(255, 255, 255, 5));
        selectionBox.setOutlineColor(sf::Color::Black);
        selectionBox.setOutlineThickness(1);

        sf::Vector2f currentMousePos = common::mouse_pos_view;
        sf::Vector2f size = currentMousePos - startSelection;

        selectionBox.setPosition(startSelection);
        selectionBox.setSize(size);

        window.draw(selectionBox);
    }
}