#include "Camera.h"

Camera::Camera()
    : is_panning(false)
    , is_moved_to_focus (false)
    , auto_focusing     (false)
{
    window_size = sf::Vector2f(WindowSettings::getInstance().settings.resolution.width, WindowSettings::getInstance().settings.resolution.height);
}

void Camera::setDefaultView() const noexcept
{
    common::view.setSize(window_size);
    common::view.setCenter(window_size / 2.f);
}

void Camera::update(const float& dtime) noexcept
{
    move(dtime);
    focusOn();
}

void Camera::move(const float& dtime) noexcept
{
    float zoom_factor    = common::view.getSize().x / WindowSettings::getInstance().settings.resolution.width;
    float adjusted_speed = WindowSettings::getInstance().settings.camera_speed * zoom_factor;

    if (sf::Keyboard::isKeyPressed(camera_controls.up))    { common::view.move(0,-adjusted_speed * dtime   ); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(camera_controls.down))  { common::view.move(0, adjusted_speed * dtime   ); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(camera_controls.left))  { common::view.move(  -adjusted_speed * dtime, 0); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(camera_controls.right)) { common::view.move(   adjusted_speed * dtime, 0); stopFocus(); }
}

void Camera::zoom() noexcept
{
    if (common::sfml_event.type == sf::Event::MouseWheelScrolled)
    {
        if (common::sfml_event.mouseWheelScroll.delta > 0)
        {
            if (common::view.getSize().x <= camera_settings.max_zoom || common::view.getSize().y <= camera_settings.max_zoom)
                common::view.zoom(1.1);
        }
        else if (common::sfml_event.mouseWheelScroll.delta < 0)
        {
            if (common::view.getSize().x >= camera_settings.min_zoom || common::view.getSize().y >= camera_settings.min_zoom)
                common::view.zoom(0.9);
        }
    }
}

void Camera::scroll() noexcept
{
    if (common::sfml_event.type == sf::Event::MouseButtonPressed &&
        common::sfml_event.mouseButton.button == sf::Mouse::Middle)
    {
        prev_mouse_pos = static_cast<sf::Vector2f>(common::mouse_pos_window);
        is_panning = true;
        stopFocus();
    }
    else if (common::sfml_event.type == sf::Event::MouseMoved && is_panning)
    {
        sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(common::mouse_pos_window);
        sf::Vector2f offset   = prev_mouse_pos - mouse_pos;
        prev_mouse_pos        = mouse_pos;

        if (offset.x * offset.x + offset.y * offset.y > camera_settings.pan_threshold * camera_settings.pan_threshold)
            common::view.move(offset);

        stopFocus();
    }
    else if (common::sfml_event.type == sf::Event::MouseButtonReleased &&
             common::sfml_event.mouseButton.button == sf::Mouse::Middle)
    {
        is_panning = false;
    }
}

void Camera::focusOn() noexcept
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

        float move_speed = 1.0f;
        float move_step  = move_speed * smooth;

        if (distance < move_step)
            common::view.setCenter(target_position);
        else
            common::view.move(direction * move_step);
    }
}

bool Camera::enableFocus(const bool approve) noexcept
{
    return is_moved_to_focus = approve;
}

void Camera::stopFocus() noexcept
{
    auto_focusing = false;
    enableFocus(false);
}

const bool Camera::isMoveToFocus() const noexcept
{
    return is_moved_to_focus;
}

sf::Vector2f Camera::setFocusOn(sf::Vector2f target) noexcept
{
    return target_position = target;
}

void Camera::reset() noexcept
{
    auto_focusing     = false;
    is_moved_to_focus = false;

    setDefaultView();
}