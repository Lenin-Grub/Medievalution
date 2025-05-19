#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
    : max_zoom          (1500)
    , min_zoom          (90)
    , pan_threshold     (5.0f)
    , arrival_threshold (1.0f)
    , is_panning        (false)
    , is_moved_to_focus (false)
    , auto_focusing     (false)
{
}

void Camera::setDefaulatView()
{
    common::view.setSize(sf::Vector2f(WindowSettings::getInstance().settings.resolution.width,
        WindowSettings::getInstance().settings.resolution.height));

    common::view.setCenter(sf::Vector2f(WindowSettings::getInstance().settings.resolution.width / 2.f,
        WindowSettings::getInstance().settings.resolution.height / 2.f));
}

void Camera::update(const float& dtime)
{
    move(dtime);
    focusOn();
}

void Camera::move(const float& dtime)
{
    float zoom_factor    = common::view.getSize().x / WindowSettings::getInstance().settings.resolution.width;
    float adjusted_speed = WindowSettings::getInstance().settings.camera_speed * zoom_factor;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { common::view.move(  -adjusted_speed * dtime, 0); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { common::view.move(   adjusted_speed * dtime, 0); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { common::view.move(0,-adjusted_speed * dtime   ); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { common::view.move(0, adjusted_speed * dtime   ); stopFocus(); }
}

void Camera::zoom() 
{
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        if (common::sfml_event.type == sf::Event::MouseWheelScrolled)
        {
            if (common::sfml_event.mouseWheelScroll.delta > 0)
            {
                if (common::view.getSize().x <= max_zoom || common::view.getSize().y <= max_zoom)
                    common::view.zoom(1.1 + WindowSettings::getInstance().settings.zoom_speed);
            }
            else if (common::sfml_event.mouseWheelScroll.delta < 0)
            {
                if (common::view.getSize().x >= min_zoom || common::view.getSize().y >= min_zoom)
                    common::view.zoom(0.9 - WindowSettings::getInstance().settings.zoom_speed);
            }
        }
    }
}

void Camera::scroll()
{
    if (common::sfml_event.type == sf::Event::MouseButtonPressed && common::sfml_event.mouseButton.button == sf::Mouse::Middle) 
    {
        prev_mouse_pos.x = common::mouse_pos_window.x;
        prev_mouse_pos.y = common::mouse_pos_window.y;
        is_panning       = true;
        stopFocus();
    }
    else if (common::sfml_event.type == sf::Event::MouseMoved && is_panning) 
    {
        sf::Vector2f mouse_pos = (sf::Vector2f)common::mouse_pos_window;        
        sf::Vector2f offset    = prev_mouse_pos - mouse_pos;
        prev_mouse_pos         = mouse_pos;
        
        if (offset.x * offset.x + offset.y * offset.y > pan_threshold * pan_threshold)
            common::view.move(offset);

        stopFocus();
    }
    else if (common::sfml_event.type == sf::Event::MouseButtonReleased && common::sfml_event.mouseButton.button == sf::Mouse::Middle) 
    {
        is_panning = false;
    }
}

void Camera::focusOn()
{
    sf::Vector2f current_сenter = common::view.getCenter();
    sf::Vector2f direction      = target_position - current_сenter;
    float        smooth         = 50.0f;
    float        distance       = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (!isMoveToFocus())
        return;

    auto_focusing = true;
    target_focus = target_position;

    // Normalize
    if (distance > 0.001f)
    {
        direction /= distance;

        float move_speed = WindowSettings::getInstance().settings.camera_speed;
        float move_step = move_speed * smooth;

        if (distance < move_step)
            common::view.setCenter(target_position);
        else
            common::view.move(direction * move_step);
    }
}

bool Camera::enableFocus(const bool approve)
{
    return is_moved_to_focus = approve;
}

void Camera::stopFocus()
{
    auto_focusing = false;
    enableFocus(false);
}

const bool Camera::isMoveToFocus() const
{
    return is_moved_to_focus;
}

sf::Vector2f Camera::setFocusOn(sf::Vector2f target)
{
    return target_position = target;
}

void Camera::reset()
{
    auto_focusing     = false;
    is_moved_to_focus = false;

    setDefaulatView();
}
