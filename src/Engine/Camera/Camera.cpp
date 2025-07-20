#include "Camera.h"

Camera::Camera(sf::Vector2f window_size, sf::View& view)
    : is_panning (false)
    , is_moved_to_focus (false)
    , auto_focusing (false)
    , is_bounded(false)
    , window_size (window_size)
    , view(view)
{
}

void Camera::setDefaultView() const noexcept
{
    view.setSize(window_size);
    view.setCenter(window_size / 2.f);
}

void Camera::update(const float& dtime) noexcept
{
    move(dtime);
    focusOn();
}

void Camera::move(const float& dtime) noexcept
{
    float zoom_factor    = view.getSize().x / WindowSettings::getInstance().settings.resolution.width;
    //float adjusted_speed = WindowSettings::getInstance().settings.camera_speed * zoom_factor;
    float adjusted_speed = 1000 * zoom_factor;

    if (sf::Keyboard::isKeyPressed(camera_controls.up))    { view.move(0,-adjusted_speed * dtime   ); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(camera_controls.down))  { view.move(0, adjusted_speed * dtime   ); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(camera_controls.left))  { view.move(  -adjusted_speed * dtime, 0); stopFocus(); }
    if (sf::Keyboard::isKeyPressed(camera_controls.right)) { view.move(   adjusted_speed * dtime, 0); stopFocus(); }
}

void Camera::zoom() noexcept
{
    if (common::sfml_event.type == sf::Event::MouseWheelScrolled)
    {
        if (common::sfml_event.mouseWheelScroll.delta > 0)
        {
            if (view.getSize().x <= camera_settings.max_zoom || view.getSize().y <= camera_settings.max_zoom)
                view.zoom(1.1);
        }
        else if (common::sfml_event.mouseWheelScroll.delta < 0)
        {
            if (view.getSize().x >= camera_settings.min_zoom || view.getSize().y >= camera_settings.min_zoom)
                view.zoom(0.9);
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
        sf::Vector2f offset    = prev_mouse_pos - mouse_pos;
        prev_mouse_pos         = mouse_pos;

        if (offset.x * offset.x + offset.y * offset.y > camera_settings.pan_threshold * camera_settings.pan_threshold)
            view.move(offset);

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
    sf::Vector2f current_center = view.getCenter();
    sf::Vector2f direction      = target_position - current_center;
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
            view.setCenter(target_position);
        else
            view.move(direction * move_step);
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

void Camera::setWindowSize(sf::Vector2f size) noexcept
{
    window_size = size;
}

void Camera::setSettings(CameraSettings settings) noexcept
{
    camera_settings = settings;
}

void Camera::setSpeed(float speed) noexcept
{
    camera_settings.movement_speed = speed;
}

void Camera::setBounds(const sf::FloatRect& bounds) noexcept
{
    camera_bounds = bounds;
    is_bounded    = true;
}

void Camera::enableBounds(bool enable) noexcept
{
    is_bounded = enable;
}

void Camera::clampToBounds(const sf::FloatRect& world_bounds) noexcept
{
    if (!is_bounded)
        return;

    sf::Vector2f view_size = view.getSize();
    sf::Vector2f view_center = view.getCenter();

    float min_x = world_bounds.left + view_size.x / 2.f;
    float max_x = world_bounds.left + world_bounds.width - view_size.x / 2.f;
    float min_y = world_bounds.top + view_size.y / 2.f;
    float max_y = world_bounds.top + world_bounds.height - view_size.y / 2.f;

    float clamped_x = std::clamp(view_center.x, min_x, max_x);
    float clamped_y = std::clamp(view_center.y, min_y, max_y);

    if (clamped_x != view_center.x || clamped_y != view_center.y)
    {
        view.setCenter(clamped_x, clamped_y);
    }
}