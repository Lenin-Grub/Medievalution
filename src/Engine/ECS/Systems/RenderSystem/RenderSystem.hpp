#pragma once
#include "../ISystem.hpp"

class RenderSystem
{
public:
    static void render(entt::registry& registry, sf::RenderTarget& window);

private:
    static void selectionBox(sf::RenderTarget& window);
    static void drawSelectionMarker(sf::RenderTarget& window, const Components::Position& position, const Components::Sprite& sprite);
};