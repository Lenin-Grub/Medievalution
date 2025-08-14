#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Math/Math.h>
#include <ECS/Entity/Entity.hpp>

enum class GizmoMode
{
    None = 0,
    Translate,
    Rotate,
    Scale
};

class Gizmo
{
public:
    Gizmo(Registry& registry);
    ~Gizmo() = default;

    void setTarget(entt::entity entity, entt::registry& registry);
    void setMode(GizmoMode mode);
    GizmoMode getMode() const;

    void update(entt::registry& registry, const sf::Vector2f& mouse_pos, bool mouse_pressed);
    void render(entt::registry& registry, sf::RenderTarget& target);

    bool isActive() const;
    void deactivate();


private:
    void drawAxes(const sf::Vector2f& center, sf::RenderTarget& target);
    void drawTranslateHandles(const sf::Vector2f& center, sf::RenderTarget& target);
    void drawScaleHandles(const sf::Vector2f& center, sf::RenderTarget& target);
    void drawRotateHandle(const sf::Vector2f& center, sf::RenderTarget& target);
    void drawDashedLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::RenderTarget& target);

private:
    void handleInput(entt::registry& registry, const sf::Vector2f& mouse_pos, bool mouse_pressed);
    void startDragging(const sf::Vector2f& mouse_pos, Components::Position& position, const sf::Vector2f& current_scale, float current_angle, const sf::Vector2f& center);
    void processDragging(const sf::Vector2f& mouse_pos, Components::Position& position, Components::Sprite* scale_component, Components::Position* rotation_component, const sf::Vector2f& center);
    void handleTranslation(const sf::Vector2f& mouse_pos, Components::Position& position);
    void handleScaling(const sf::Vector2f& mouse_pos, Components::Sprite* scale_component);
    void handleRotation(const sf::Vector2f& mouse_pos, Components::Position* rotation_component,const sf::Vector2f& center);

    bool isPointInXHandle(const sf::Vector2f& point, const sf::Vector2f& center);
    bool isPointInYHandle(const sf::Vector2f& point, const sf::Vector2f& center);
    bool isPointInXYHandle(const sf::Vector2f& point, const sf::Vector2f& center);

private:
    bool font_loaded = false;
    bool is_dragging = false;

    float initial_angle = 0.0f;

    sf::Vector2f drag_start;
    sf::Vector2f drag_axis;
    sf::Vector2f initial_position;
    sf::Vector2f initial_scale;
    sf::Vector2f mouse_pos;

    sf::Font font;
    sf::Text coords;

    std::string text;

    entt::entity target_entity = entt::null;
    GizmoMode mode = GizmoMode::None;
    Registry& registry;
};