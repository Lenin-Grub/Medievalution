#pragma once
#include "../ISystem.hpp"

class RenderSystem
{
public:
    static void render(entt::registry& registry, sf::RenderWindow& window);

private:
    static void selectionBox(sf::RenderWindow& window);
};