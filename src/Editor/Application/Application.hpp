#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../UI/Displays.hpp"

#include <Gizmos/Gizmos.h>

class EditorApplication
{
public:
    EditorApplication();

    virtual ~EditorApplication();

    void run();

private:
    bool init();
    void updateEvents();
    void update();
    void draw(sf::RenderTarget* target = nullptr);

private:
    void updateMousePositions(sf::View* view, sf::RenderWindow& window);
    bool initIcon(sf::RenderWindow& window);
    bool initFonts();
    void setupImGui();
    void setupWindow();
 
private:
    float             delta_time;

    sf::RenderWindow  window;
    sf::Clock         clock;
    sf::Vector2f      window_size;
    sf::Color         clear_color;

    Displays          displays;
    BattleMap         battle_map;
    Camera            camera;
    Gizmo             gizmo;

    Registry          registry;
    Animator          animator;
    Pathfinding       pathfinding;
    sf::Sprite        sprite;
};
