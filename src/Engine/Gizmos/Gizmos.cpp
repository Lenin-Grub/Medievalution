#include "Gizmos.h"
#include <filesystem>
#include <numbers>

Gizmos::Gizmos(sf::RenderWindow& window, sf::Vector2f position)
    :window(window)
    ,position(position)
    ,is_dragging(false)
    ,angle(0.0f)
    //,font(ResourceLoader::instance().getFont("OpenSans-Semibold.ttf"))
{
    coords.setFont(font);
    coords.setCharacterSize(12);
    coords.setFillColor(sf::Color::White);
}

void Gizmos::draw()
{
    sf::RectangleShape shape(sf::Vector2f(50.0f, 50.0f));
    shape.setFillColor(sf::Color(24,24,24));
    shape.setPosition(position);
    shape.setRotation(angle);
    shape.setScale(scale);

    window.draw(shape);

    sf::Vector2f center = position;
    drawAxes(center);
    drawArrows(center);
    drawScaleHandles(center);
    drawRotationHandle(center);

    if (is_dragging)
        drawDashedLine(window, initial_position, position, 4, 4, 2);
}

void Gizmos::drawImGui()
{
 /*   float available_width = ImGui::GetContentRegionAvail().x;
    float button_width = 30.0f;
    float slider_width = (available_width - 2 * button_width - 30) / 2;

        if (mode == GizmoMode::Translate) 
        {
            ImGui::Text("Translate Mode");

            ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Red);
            ImGui::Button("X", ImVec2(button_width, 0));
            ImGui::PopStyleColor();
            ImGui::SameLine(0, 1);
            ImGui::SetNextItemWidth(slider_width);
            ImGui::SliderFloat("##PositionX", &position.x, 0.0f, window.getSize().x, "%.0f");

            ImGui::SameLine(0, 10);
            ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
            ImGui::Button("Y", ImVec2(button_width, 0));
            ImGui::PopStyleColor();
            ImGui::SameLine(0, 1);
            ImGui::SetNextItemWidth(slider_width);
            ImGui::SliderFloat("##PositionY", &position.y, 0.0f, window.getSize().y, "%.0f");
        }

        if (mode == GizmoMode::Scale)
        {
            ImGui::Text("Scale Mode");

            ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Red);
            ImGui::Button("X", ImVec2(button_width, 0));
            ImGui::PopStyleColor();
            ImGui::SameLine(0, 1);
            ImGui::SetNextItemWidth(slider_width);
            ImGui::SliderFloat("##ScaleX", &scale.x, 0.1f, 2.0f, "%.2f");

            ImGui::SameLine(0, 10);
            ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
            ImGui::Button("Y", ImVec2(button_width, 0));
            ImGui::PopStyleColor();
            ImGui::SameLine(0, 1);
            ImGui::SetNextItemWidth(slider_width);
            ImGui::SliderFloat("##ScaleY", &scale.y, 0.1f, 2.0f, "%.2f");
        }

    if (mode == GizmoMode::Rotate) 
    {
        ImGui::Text("Rotate Mode");
        float angleDegrees = angle;
        ImGui::SliderFloat("Angle", &angleDegrees, 0.0f, 360.0f, "%.1f deg");
        angle = angleDegrees;
    }*/
}


void Gizmos::update()
{
    sf::Vector2f center = position;
    mouseDragging(center);
}

void Gizmos::drawAxes(const sf::Vector2f& center)
{
    if (mode != GizmoMode::Rotate && mode != GizmoMode::None)
    {
        sf::RectangleShape lineX(sf::Vector2f(100.0f, 4.0f));
        lineX.setFillColor(sf::Color::Red);
        lineX.setPosition(center - sf::Vector2f(0.0f, 2.0f));

        sf::RectangleShape lineY(sf::Vector2f(4.0f, 100.0f));
        lineY.setFillColor(sf::Color::Green);
        lineY.setPosition(center - sf::Vector2f(2.0f, 0.0f));

        window.draw(lineX);
        window.draw(lineY);
    }
}

void Gizmos::drawArrows(const sf::Vector2f& center)
{
    if (mode == GizmoMode::Translate) 
    {
        sf::ConvexShape arrowX;
        arrowX.setPointCount(3);
        arrowX.setPoint(0, center + sf::Vector2f(110.0f, 0.0f));
        arrowX.setPoint(1, center + sf::Vector2f(95.0f, -10.0f));
        arrowX.setPoint(2, center + sf::Vector2f(95.0f, 10.0f));
        arrowX.setFillColor(sf::Color::Red);

        sf::ConvexShape arrowY;
        arrowY.setPointCount(3);
        arrowY.setPoint(0, center + sf::Vector2f(0.0f, 110.0f));
        arrowY.setPoint(1, center + sf::Vector2f(-10.0f, 95.0f));
        arrowY.setPoint(2, center + sf::Vector2f(10.0f, 95.0f));
        arrowY.setFillColor(sf::Color::Green);

        sf::RectangleShape rectXY(sf::Vector2f(25.0f, 25.0f));
        rectXY.setPosition(center + sf::Vector2f(2, 2));
        rectXY.setFillColor(sf::Color(234, 142, 66, 50));
        rectXY.setOutlineColor(sf::Color(234, 142, 66));
        rectXY.setOutlineThickness(4);

        window.draw(arrowX);
        window.draw(arrowY);
        window.draw(rectXY);
    }
}

void Gizmos::drawScaleHandles(const sf::Vector2f& center)
{
    if (mode == GizmoMode::Scale) 
    {
        sf::RectangleShape rectX(sf::Vector2f(15.0f, 15.0f));
        rectX.setFillColor(sf::Color::Red);
        rectX.setPosition(center + sf::Vector2f(90.0f, -7.5f));

        sf::RectangleShape rectY(sf::Vector2f(15.0f, 15.0f));
        rectY.setFillColor(sf::Color::Green);
        rectY.setPosition(center + sf::Vector2f(-7.5f, 90.0f));

        sf::RectangleShape rectXY(sf::Vector2f(25.0f, 25.0f));
        rectXY.setPosition(center + sf::Vector2f(2, 2));
        rectXY.setFillColor(sf::Color(234, 142, 66, 50));
        rectXY.setOutlineColor(sf::Color(234, 142, 66));
        rectXY.setOutlineThickness(4);

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "{x " << scale.x << "  , y " << scale.y << "}";
        text = oss.str();

        coords.setString(text);
        coords.setPosition(center - sf::Vector2f(0, 20));

        window.draw(rectX);
        window.draw(rectY);
        window.draw(rectXY);
        window.draw(coords);
    }
}

void Gizmos::drawRotationHandle(const sf::Vector2f& center)
{
    if (mode == GizmoMode::Rotate) 
    {
        float size = 50.0f;
        sf::CircleShape circle(size);
        circle.setFillColor(sf::Color(234, 142, 66, 50));
        circle.setOutlineColor(sf::Color(234, 142, 66));
        circle.setOutlineThickness(4);
        circle.setPosition(center - sf::Vector2f(size, size));

        sf::CircleShape stick(7.0f);
        stick.setFillColor(sf::Color(40, 159, 49));
        stick.setOrigin(stick.getRadius(), stick.getRadius());
        stick.setPosition(center + sf::Vector2f(size * cos(angle * std::numbers::pi / 180.0f), size * sin(angle * std::numbers::pi / 180.0f)));

        sf::VertexArray sector(sf::TriangleFan, 102);
        sector[0].position = center;
        sector[0].color = sf::Color(234, 142, 66, 150);

        for (int i = 1; i <= 101; ++i)
        {
            float cur_angle = (i - 1) * (angle / 100.0f) * std::numbers::pi / 180.0f;
            sector[i].position = sf::Vector2f(center.x + circle.getRadius() * cos(cur_angle), center.y + circle.getRadius() * sin(cur_angle));
            sector[i].color    = sf::Color(234, 142, 66, 150);
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << angle << " deg";
        text = oss.str();

        coords.setString(text);
        coords.setPosition(center - sf::Vector2f(24, 6));

        window.draw(circle);
        window.draw(sector);
        window.draw(stick);
        window.draw(coords);
    }
}

void Gizmos::mouseDragging(const sf::Vector2f& center) 
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (!is_dragging) 
        {
            drag_start       = mousePos;
            initial_position = position;
            is_dragging      = true;

            if (mode == GizmoMode::Translate || mode == GizmoMode::Scale) 
            {
                sf::FloatRect arrowXBounds(center + sf::Vector2f(90.0f, -10.0f), sf::Vector2f(20.0f, 20.0f));
                sf::FloatRect arrowYBounds(center + sf::Vector2f(-10.0f, 90.0f), sf::Vector2f(20.0f, 20.0f));
                sf::FloatRect rectBounds  (center, sf::Vector2f(25.0f, 25.0f));
                sf::FloatRect lineXBounds (line_x.getGlobalBounds());
                sf::FloatRect lineYBounds (line_y.getGlobalBounds());

                if (arrowXBounds.contains(mousePos) || lineXBounds.contains(mousePos)) 
                {
                    drag_axis = sf::Vector2f(1.0f, 0.0f);
                }
                else if (arrowYBounds.contains(mousePos) || lineYBounds.contains(mousePos)) 
                {
                    drag_axis = sf::Vector2f(0.0f, 1.0f);
                }
                else if (rectBounds.contains(mousePos)) 
                {
                    drag_axis = sf::Vector2f(1.0f, 1.0f);
                }
            }
        }
        else 
        {
            sf::Vector2f delta = mousePos - drag_start;
            if (mode == GizmoMode::Translate)
            {
                position += sf::Vector2f(delta.x * drag_axis.x, delta.y * drag_axis.y);
            }
            else if (mode == GizmoMode::Scale)
            {
                scale += sf::Vector2f(delta.x * 0.01f * drag_axis.x, delta.y * 0.01f * drag_axis.y);
            }
            else if (mode == GizmoMode::Rotate)
            {
                float deltaAngle = atan2(mousePos.y - center.y, mousePos.x - center.x) - atan2(drag_start.y - center.y, drag_start.x - center.x);
                angle += deltaAngle * 180.0f / std::numbers::pi;

                if (angle < 0.0f)
                    angle += 360.0f;
                else if (angle >= 360.0f)
                    angle -= 360.0f;
            }
            drag_start = mousePos;
        }
    }
    else 
    {
        is_dragging = false;
        drag_axis   = sf::Vector2f(0.0f, 0.0f);
    }
}

void Gizmos::drawDashedLine(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end, float dashLength, float gapLength, float thickness)
{
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length == 0) 
        return;
    direction /= length;

    sf::Vector2f perpendicular(-direction.y * thickness / 2, direction.x * thickness / 2);
    sf::Color gray{ 255,255,255, 100 };
   
    float currentLength = 0.0f;

    sf::VertexArray dashes(sf::PrimitiveType::Quads);
    sf::CircleShape start_circle(2);
    start_circle.setFillColor(gray);
    start_circle.setOutlineColor(sf::Color::White);
    start_circle.setOutlineThickness(1);
    start_circle.setOrigin(start_circle.getRadius(), start_circle.getRadius());
    start_circle.setPosition(start);

    sf::CircleShape end_circle(2);
    end_circle.setFillColor(gray);
    end_circle.setOutlineColor(sf::Color::White);
    end_circle.setOutlineThickness(1);
    end_circle.setOrigin(end_circle.getRadius(), end_circle.getRadius());
    end_circle.setPosition(end);

    while (currentLength < length)
    {
        sf::Vector2f dashStart = start     + direction * currentLength;
        sf::Vector2f dashEnd   = dashStart + direction * dashLength;

        dashes.append(sf::Vertex(dashStart + perpendicular, gray));
        dashes.append(sf::Vertex(dashStart - perpendicular, gray));
        dashes.append(sf::Vertex(dashEnd   - perpendicular, gray));
        dashes.append(sf::Vertex(dashEnd   + perpendicular, gray));

        currentLength += dashLength + gapLength;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << "{x " << end.x << "  , y " << end.y << "}";
    text = oss.str();

    coords.setString(text);
    coords.setPosition(end - sf::Vector2f(0, 20));

    window.draw(dashes);
    window.draw(start_circle);
    window.draw(end_circle);
    window.draw(coords);
}