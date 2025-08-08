#include  "RenderSystem.hpp"

void RenderSystem::render(entt::registry& registry, sf::RenderTarget& window)
{
    auto view = registry.view<Components::Sprite, Components::Position>();

    std::vector<std::tuple<entt::entity, float, float>> sorted_entities;
    for (auto entity : view)
    {
        const auto& position = view.get<Components::Position>(entity);
        sorted_entities.emplace_back(entity, position.position.x, position.position.y);
    }

    std::sort(sorted_entities.begin(), sorted_entities.end(),
        [](const auto& first, const auto& second)
        {
            auto [entity_first, current_x, current_y] = first;
            auto [entity_second, next_x, next_y] = second;

            if (current_y != next_y)
                return current_y < next_y;

            return current_x < next_x;
        });

    for (const auto& [entity, x, y] : sorted_entities)
    {
        const auto& sprite_component = view.get<Components::Sprite>(entity);

        if (sprite_component.sprite.getTexture() == nullptr) 
        {
            LOG_WARN("Entity has not texture");
            sf::RectangleShape placeholder(sf::Vector2f(32, 32));
            placeholder.setFillColor(sf::Color::Red);
            placeholder.setPosition(x, y);
            window.draw(placeholder);
        }
        else 
        {
            sf::Sprite drawableSprite = sprite_component.sprite;
            drawableSprite.setPosition(x, y);
            window.draw(drawableSprite);
        }

        if (registry.all_of<Components::Selectable>(entity))
        {
            const auto& selectable_component = registry.get<Components::Selectable>(entity);
            if (selectable_component.is_selected)
            {
                const auto& position_component = view.get<Components::Position>(entity);
                drawSelectionMarker(window, position_component, sprite_component);
            }
        }
    }

    selectionBox(window);
}

void RenderSystem::selectionBox(sf::RenderTarget& window)
{
    static sf::Vector2f start_selection;
    static bool is_selecting = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !is_selecting)
    {
        start_selection = common::mouse_pos_view;
        is_selecting = true;
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && is_selecting)
        is_selecting = false;

    if (!is_selecting)
        return;

    sf::RectangleShape selection_box;
    selection_box.setFillColor(sf::Color(255, 255, 255, 5));
    selection_box.setOutlineColor(sf::Color::Black);
    selection_box.setOutlineThickness(1);

    sf::Vector2f current_mouse_pos = common::mouse_pos_view;
    sf::Vector2f size = current_mouse_pos - start_selection;

    selection_box.setPosition(start_selection);
    selection_box.setSize(size);

    window.draw(selection_box);
}

void RenderSystem::drawSelectionMarker(sf::RenderTarget& window, const Components::Position& position, const Components::Sprite& sprite)
{
    int size = 5;
    sf::ConvexShape diamond(4);
    diamond.setFillColor(sf::Color::Green);
    diamond.setPoint(0, sf::Vector2f(0, -size));
    diamond.setPoint(1, sf::Vector2f(size, 0));
    diamond.setPoint(2, sf::Vector2f(0, size));
    diamond.setPoint(3, sf::Vector2f(-size, 0));

    sf::Vector2f marker_position = position.position;

    // TO DO: remove magic numbers
    marker_position.x -= sprite.sprite.getGlobalBounds().width / 2.f - 48.f;
    marker_position.y -= sprite.sprite.getGlobalBounds().height / 2.f - 12.f;

    diamond.setPosition(marker_position);
    window.draw(diamond);
}