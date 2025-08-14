#include "Gizmos.h"

#include <filesystem>
#include <numbers>

#include <ECS/Components/Components.hpp>
#include <entt/entt.hpp>

Gizmo::Gizmo(Registry& registry)
    :registry(registry)
{
    //TODO bad very bad

    font = ResourceLoader::instance().getFont("OpenSans-Semibold.ttf");

    coords.setFont(font);
    coords.setCharacterSize(12);
    coords.setFillColor(sf::Color::White);
    font_loaded = true;
}

void Gizmo::setTarget(entt::entity entity, entt::registry& registry)
{
    if (registry.valid(entity))
        target_entity = entity;
}

void Gizmo::setMode(GizmoMode new_mode)
{
    mode        = new_mode;
    is_dragging = false;
}

GizmoMode Gizmo::getMode() const 
{
    return mode;
}

bool Gizmo::isActive() const
{
    return target_entity != entt::null && mode != GizmoMode::None;
}

void Gizmo::deactivate()
{
    target_entity = entt::null;
    mode          = GizmoMode::None;
    is_dragging   = false;
}

void Gizmo::update(entt::registry& registry, const sf::Vector2f& mouse_pos, bool mouse_pressed)
{
    if (!isActive()) 
        return;

    handleInput(registry, mouse_pos, mouse_pressed);
}

void Gizmo::render(entt::registry& registry, sf::RenderTarget& target)
{
    if (!isActive() || !registry.valid(target_entity)) 
        return;

    auto& position      = registry.get<Components::Position>(target_entity);
    sf::Vector2f center = position.position;

    drawAxes(center, target);

    switch (mode) 
    {
    case GizmoMode::Translate:
        drawTranslateHandles(center, target);
        break;
    case GizmoMode::Scale:
        drawScaleHandles(center, target);
        break;
    case GizmoMode::Rotate:
        drawRotateHandle(center, target);
        break;
    default:
        break;
    }

    if (is_dragging) 
        drawDashedLine(initial_position, position.position, target);
}

void Gizmo::drawAxes(const sf::Vector2f& center, sf::RenderTarget& target)
{
    if (mode != GizmoMode::Rotate && mode != GizmoMode::None)
    {
        sf::RectangleShape lineX(sf::Vector2f(100.0f, 4.0f));
        lineX.setFillColor(sf::Color::Red);
        lineX.setPosition(center - sf::Vector2f(0.0f, 2.0f));
        target.draw(lineX);

        sf::RectangleShape lineY(sf::Vector2f(4.0f, 100.0f));
        lineY.setFillColor(sf::Color::Green);
        lineY.setPosition(center - sf::Vector2f(2.0f, 0.0f));
        target.draw(lineY);
    }
}

void Gizmo::drawTranslateHandles(const sf::Vector2f& center, sf::RenderTarget& target)
{
    if (mode == GizmoMode::Translate)
    {
        sf::ConvexShape arrowX;
        arrowX.setPointCount(3);
        arrowX.setPoint(0, center + sf::Vector2f(110.0f, 0.0f));
        arrowX.setPoint(1, center + sf::Vector2f(95.0f, -10.0f));
        arrowX.setPoint(2, center + sf::Vector2f(95.0f, 10.0f));
        arrowX.setFillColor(sf::Color::Red);
        target.draw(arrowX);

        sf::ConvexShape arrowY;
        arrowY.setPointCount(3);
        arrowY.setPoint(0, center + sf::Vector2f(0.0f, 110.0f));
        arrowY.setPoint(1, center + sf::Vector2f(-10.0f, 95.0f));
        arrowY.setPoint(2, center + sf::Vector2f(10.0f, 95.0f));
        arrowY.setFillColor(sf::Color::Green);
        target.draw(arrowY);

        sf::RectangleShape rectXY(sf::Vector2f(25.0f, 25.0f));
        rectXY.setPosition(center + sf::Vector2f(2, 2));
        rectXY.setFillColor(sf::Color(234, 142, 66, 50));
        rectXY.setOutlineColor(sf::Color(234, 142, 66));
        rectXY.setOutlineThickness(4);
        target.draw(rectXY);

        if (font_loaded && target_entity != entt::null)
        {
            auto& position_component = registry.getRegistry().get<Components::Position>(target_entity);
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << "{x " << position_component.position.x << "  , y " << position_component.position.y << "}";
            text = oss.str();

            coords.setString(text);
            coords.setPosition(center - sf::Vector2f(0, 20));
            target.draw(coords);
        }
    }
}


void Gizmo::drawScaleHandles(const sf::Vector2f& center, sf::RenderTarget& target)
{
    if (mode == GizmoMode::Scale)
    {
        sf::RectangleShape rectX(sf::Vector2f(15.0f, 15.0f));
        rectX.setFillColor(sf::Color::Red);
        rectX.setPosition(center + sf::Vector2f(90.0f, -7.5f));
        target.draw(rectX);

        sf::RectangleShape rectY(sf::Vector2f(15.0f, 15.0f));
        rectY.setFillColor(sf::Color::Green);
        rectY.setPosition(center + sf::Vector2f(-7.5f, 90.0f));
        target.draw(rectY);

        sf::RectangleShape rectXY(sf::Vector2f(25.0f, 25.0f));
        rectXY.setPosition(center + sf::Vector2f(2, 2));
        rectXY.setFillColor(sf::Color(234, 142, 66, 50));
        rectXY.setOutlineColor(sf::Color(234, 142, 66));
        rectXY.setOutlineThickness(4);
        target.draw(rectXY);

        if (target_entity != entt::null)
        {
            auto* scale_component = registry.getRegistry().try_get<Components::Sprite>(target_entity);
            if (scale_component)
            {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(1) << "{x " << scale_component->scale.x << "  , y " << scale_component->scale.y << "}";
                text = oss.str();

                coords.setString(text);
                coords.setPosition(center - sf::Vector2f(0, 20));
                target.draw(coords);
            }
        }
    }
}

void Gizmo::drawRotateHandle(const sf::Vector2f& center, sf::RenderTarget& target)
{
    if (mode == GizmoMode::Rotate)
    {
        float size = 50.0f;

        sf::CircleShape circle(size);
        circle.setFillColor(sf::Color(234, 142, 66, 50));
        circle.setOutlineColor(sf::Color(234, 142, 66));
        circle.setOutlineThickness(4);
        circle.setPosition(center - sf::Vector2f(size, size));

        float actual_angle = initial_angle;
        if (target_entity != entt::null) {
            auto* rotation_component = registry.getRegistry().try_get<Components::Position>(target_entity);
            if (rotation_component)
                actual_angle = rotation_component->rotation;
        }

        sf::CircleShape stick(7.0f);
        stick.setFillColor(sf::Color(40, 159, 49));
        stick.setOrigin(stick.getRadius(), stick.getRadius());

        float current_angle_rad = actual_angle * std::numbers::pi / 180.0f;
        stick.setPosition(center + sf::Vector2f(size * cos(current_angle_rad), size * sin(current_angle_rad)));

        sf::VertexArray sector(sf::TriangleFan, 102);
        sector[0].position = center;
        sector[0].color = sf::Color(234, 142, 66, 150);

        for (int i = 1; i <= 101; ++i)
        {
            float cur_angle = (i - 1) * (actual_angle / 100.0f) * std::numbers::pi / 180.0f;
            sector[i].position = sf::Vector2f(center.x + size * cos(cur_angle), center.y + size * sin(cur_angle));
            sector[i].color = sf::Color(234, 142, 66, 150);
        }

        target.draw(circle);
        target.draw(sector);
        target.draw(stick);

        if (font_loaded) 
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << actual_angle << " deg";
            text = oss.str();

            coords.setString(text);
            coords.setPosition(center - sf::Vector2f(24, 6));
            target.draw(coords);
        }
    }
}

bool Gizmo::isPointInXHandle(const sf::Vector2f& point, const sf::Vector2f& center)
{
    sf::FloatRect bounds(center + sf::Vector2f(95.0f, -10.0f), sf::Vector2f(15.0f, 20.0f));
    return bounds.contains(point);
}

bool Gizmo::isPointInYHandle(const sf::Vector2f& point, const sf::Vector2f& center)
{
    sf::FloatRect bounds(center + sf::Vector2f(-10.0f, 95.0f), sf::Vector2f(20.0f, 15.0f));
    return bounds.contains(point);
}

bool Gizmo::isPointInXYHandle(const sf::Vector2f& point, const sf::Vector2f& center)
{
    sf::FloatRect bounds(center, sf::Vector2f(25.0f, 25.0f));
    return bounds.contains(point);
}

void Gizmo::handleInput(entt::registry& registry, const sf::Vector2f& mouse_pos, bool mouse_pressed)
{
    this->mouse_pos = mouse_pos;

    if (!registry.valid(target_entity))
        return;

    auto& position = registry.get<Components::Position>(target_entity);
    auto* scale_component = registry.try_get<Components::Sprite>(target_entity);
    auto* rotation_component = registry.try_get<Components::Position>(target_entity);

    sf::Vector2f center = position.position;
    float current_angle = rotation_component ? rotation_component->rotation : 0.0f;
    sf::Vector2f current_scale = scale_component ? scale_component->scale : sf::Vector2f(1.0f, 1.0f);

    if (!mouse_pressed) 
    {
        is_dragging = false;
        drag_axis = sf::Vector2f(0.0f, 0.0f);
        return;
    }

    if (!is_dragging)
    {
        startDragging(mouse_pos, position, current_scale, current_angle, center);
        return;
    }

    processDragging(mouse_pos, position, scale_component, rotation_component, center);
}

void Gizmo::startDragging(const sf::Vector2f& mouse_pos, Components::Position& position,
    const sf::Vector2f& current_scale, float current_angle, const sf::Vector2f& center)
{
    drag_start = mouse_pos;
    initial_position = position.position;
    initial_scale = current_scale;
    initial_angle = current_angle;
    is_dragging = true;

    if (mode == GizmoMode::Translate || mode == GizmoMode::Scale)
    {
        if (isPointInXHandle(mouse_pos, center)) 
        {
            drag_axis = sf::Vector2f(1.0f, 0.0f);
        }
        else if (isPointInYHandle(mouse_pos, center)) 
        {
            drag_axis = sf::Vector2f(0.0f, 1.0f);
        }
        else if (isPointInXYHandle(mouse_pos, center)) 
        {
            drag_axis = sf::Vector2f(1.0f, 1.0f);
        }
        else 
        {
            is_dragging = false;
        }
    }
    else if (mode == GizmoMode::Rotate) 
    {
        drag_axis = sf::Vector2f(1.0f, 1.0f);
    }
}

void Gizmo::processDragging(const sf::Vector2f& mouse_pos, Components::Position& position, Components::Sprite* scale_component, Components::Position* rotation_component, const sf::Vector2f& center)
{
    switch (mode) 
    {
    case GizmoMode::Translate:
        handleTranslation(mouse_pos, position);
        break;

    case GizmoMode::Scale:
        handleScaling(mouse_pos, scale_component);
        break;

    case GizmoMode::Rotate:
        handleRotation(mouse_pos, rotation_component, center);
        break;

    default:
        break;
    }

    if (mode == GizmoMode::Rotate)
        drag_start = mouse_pos;
}

void Gizmo::handleTranslation(const sf::Vector2f& mouse_pos, Components::Position& position)
{
    sf::Vector2f delta = mouse_pos - drag_start;
    position.position = initial_position + sf::Vector2f(delta.x * drag_axis.x, delta.y * drag_axis.y);
}

void Gizmo::handleScaling(const sf::Vector2f& mouse_pos, Components::Sprite* scale_component)
{
    if (!scale_component) 
        return;

    sf::Vector2f delta = mouse_pos - drag_start;
    scale_component->scale = initial_scale + sf::Vector2f(delta.x * 0.01f * drag_axis.x, delta.y * 0.01f * drag_axis.y);
    scale_component->scale.x = std::max(0.1f, scale_component->scale.x);
    scale_component->scale.y = std::max(0.1f, scale_component->scale.y);
}

void Gizmo::handleRotation(const sf::Vector2f& mouse_pos, Components::Position* rotation_component, const sf::Vector2f& center)
{
    if (!rotation_component) 
        return;

    float deltaAngle = atan2(mouse_pos.y  - center.y, mouse_pos.x  - center.x) 
                     - atan2(drag_start.y - center.y, drag_start.x - center.x);

    rotation_component->rotation += deltaAngle * 180.0f / std::numbers::pi;

    if (rotation_component->rotation < 0.0f)
        rotation_component->rotation += 360.0f;
    else if (rotation_component->rotation >= 360.0f)
        rotation_component->rotation -= 360.0f;

    initial_angle = rotation_component->rotation;
}

void Gizmo::drawDashedLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::RenderTarget& target)
{
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length == 0) 
        return;

    direction /= length;

    sf::Vector2f perpendicular(-direction.y, direction.x);
    perpendicular *= 1.0f;

    float dashLength = 5.0f;
    float gapLength = 3.0f;
    float currentLength = 0.0f;

    sf::VertexArray dashes(sf::Quads);
    sf::Color dashColor(255, 255, 255, 200);

    while (currentLength < length)
    {
        float segmentEnd = std::min(currentLength + dashLength, length);

        sf::Vector2f dashStart = start + direction * currentLength;
        sf::Vector2f dashEnd = start + direction * segmentEnd;

        dashes.append(sf::Vertex(dashStart + perpendicular, dashColor));
        dashes.append(sf::Vertex(dashStart - perpendicular, dashColor));
        dashes.append(sf::Vertex(dashEnd - perpendicular, dashColor));
        dashes.append(sf::Vertex(dashEnd + perpendicular, dashColor));

        currentLength += dashLength + gapLength;
    }

    target.draw(dashes);
}