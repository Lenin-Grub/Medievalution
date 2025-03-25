#include "../../../stdafx.h"
#include  "RenderSystem.hpp"

void RenderSystem::render(entt::registry& registry, sf::RenderWindow& window)
{
    auto view = registry.view<Components::Sprite, Components::Position, Components::Selectable>();

    for (auto entity : view)
    {
        auto& sprite_component     = view.get<Components::Sprite>     (entity);
        auto& position_component   = view.get<Components::Position>   (entity);
        auto& selectable_component = view.get<Components::Selectable> (entity);

        // Draw entityes
        window.draw(sprite_component.sprite);

        if (selectable_component.is_selected)
        {
            // Создаем ромбик
            int size = 5;
            sf::ConvexShape diamond(4);
            diamond.setFillColor(sf::Color::Green);
            diamond.setPoint(0, sf::Vector2f(0, -size));          // Верхняя точка
            diamond.setPoint(1, sf::Vector2f(size, 0));           // Правая точка
            diamond.setPoint(2, sf::Vector2f(0, size));           // Нижняя точка
            diamond.setPoint(3, sf::Vector2f(-size, 0));          // Левая точка

            // Центрируем ромбик над сущностью
            sf::Vector2f marker_position = position_component.position;
            marker_position.x -= sprite_component.sprite.getGlobalBounds().width / 2 - 48; // Выше спрайта
            marker_position.y -= sprite_component.sprite.getGlobalBounds().height / 2 - 12; // Выше спрайта
            diamond.setPosition(marker_position);

            window.draw(diamond);
        }

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