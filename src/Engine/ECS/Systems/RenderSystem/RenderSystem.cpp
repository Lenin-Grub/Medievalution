#include  "RenderSystem.hpp"

void RenderSystem::render(entt::registry& registry, sf::RenderWindow& window)
{
    auto view = registry.view<Components::Sprite, Components::Position, Components::Selectable>();

    std::vector<std::tuple<entt::entity, float, float>> sorted_entities;

    for (auto entity : view) 
    {
        auto& position_component = view.get<Components::Position>(entity);
        sorted_entities.emplace_back(entity, position_component.position.x, position_component.position.y);
    }

    // Сортируем по позиции Y, а затем по X (если Y одинаковые)
    std::sort(sorted_entities.begin(), sorted_entities.end(), [](const auto& a, const auto& b)
        {
            auto [entity_a, x_a, y_a] = a;
            auto [entity_b, x_b, y_b] = b;

            if (y_a != y_b)
            {
                return y_a < y_b;
            }
            return x_a < x_b;
        });

    for (const auto& [entity, x, y] : sorted_entities) 
    {
        auto& sprite_component = view.get<Components::Sprite>(entity);
        window.draw(sprite_component.sprite);
    }

    for (const auto& [entity, x, y] : sorted_entities) 
    {
        auto& position_component   = view.get<Components::Position>  (entity);
        auto& selectable_component = view.get<Components::Selectable>(entity);

        if (selectable_component.is_selected)
        {
            int size = 5;
            sf::ConvexShape diamond(4);
            diamond.setFillColor(sf::Color::Green);
            diamond.setPoint(0, sf::Vector2f(0, -size));
            diamond.setPoint(1, sf::Vector2f(size, 0));
            diamond.setPoint(2, sf::Vector2f(0, size));
            diamond.setPoint(3, sf::Vector2f(-size, 0));

            auto& sprite_component = view.get<Components::Sprite>(entity);
            sf::Vector2f marker_position = position_component.position;
            marker_position.x -= sprite_component.sprite.getGlobalBounds().width / 2 - 48;
            marker_position.y -= sprite_component.sprite.getGlobalBounds().height / 2 - 12;
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