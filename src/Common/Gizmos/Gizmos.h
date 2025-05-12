#pragma once
#include "stdafx.h"
#include "../../Input/Input.h"

enum class GizmoMode
{
    None,
    Translate,
    Rotate,
    Scale
};

class Gizmos
{
public:

    Gizmos(sf::RenderWindow& window, sf::Vector2f position);
    ~Gizmos() = default;

    void update();

    void draw();

    void drawImGui();

private:

    void drawAxes(const sf::Vector2f& center);

    void drawArrows(const sf::Vector2f& center);

    void drawScaleHandles(const sf::Vector2f& center);

    void drawRotationHandle(const sf::Vector2f& center);

    void mouseDragging(const sf::Vector2f& center);

    void drawDashedLine(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, float dashLength, float gapLength, float thickness);

public:

    GizmoMode          mode = GizmoMode::None;

private:
    sf::RenderWindow&  window;
    sf::Vector2f       position;
    sf::Vector2f       initial_position;
    sf::Vector2f       scale;

    sf::Vector2f       drag_start;
    sf::Vector2f       drag_axis;

    sf::RectangleShape line_x;
    sf::RectangleShape line_y;
    float              angle;
    bool               is_dragging;

    std::string        text;
    sf::Text           coords;
    sf::Font           font;
};
