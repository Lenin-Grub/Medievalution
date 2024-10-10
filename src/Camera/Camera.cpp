#include "../stdafx.h"
#include "Camera.h"

Camera::Camera()
    : max_zoom      (1500)
    , min_zoom      (90)
    , pan_threshold (5.0f)
    , is_panning    (false)
{
}

void Camera::setDefaulatView()
{
    common::view.setSize(sf::Vector2f(WindowSettings::getInstance().resolution.width,
        WindowSettings::getInstance().resolution.height));

    common::view.setCenter(sf::Vector2f(WindowSettings::getInstance().resolution.width / 2.f,
        WindowSettings::getInstance().resolution.height / 2.f));
}

void Camera::update(const float& dtime)
{
    move(dtime);
}

void Camera::move(const float& dtime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { common::view.move(-WindowSettings::getInstance().camera_speed * dtime, 0); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { common::view.move(WindowSettings::getInstance().camera_speed * dtime, 0);  }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { common::view.move(0, -WindowSettings::getInstance().camera_speed * dtime); }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { common::view.move(0, WindowSettings::getInstance().camera_speed * dtime);  }

}

void Camera::zoom() const
{
    if (!ImGui::GetIO().WantCaptureMouse)
    {
        if (common::sfml_event.type == sf::Event::MouseWheelScrolled)
        {
            if (common::sfml_event.mouseWheelScroll.delta > 0)
            {
                if (common::view.getSize().x <= max_zoom || common::view.getSize().y <= max_zoom)
                    common::view.zoom(1.1 + WindowSettings::getInstance().zoom_speed);
            }
            else if (common::sfml_event.mouseWheelScroll.delta < 0)
            {
                if (common::view.getSize().x >= min_zoom || common::view.getSize().y >= min_zoom)
                    common::view.zoom(0.9 - WindowSettings::getInstance().zoom_speed);
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
        is_panning = true;
    }
    else if (common::sfml_event.type == sf::Event::MouseMoved && is_panning) 
    {
        sf::Vector2f mousePos;
        mousePos.x = common::mouse_pos_window.x;
        mousePos.y = common::mouse_pos_window.y;
        
        sf::Vector2f offset = prev_mouse_pos - mousePos;
        prev_mouse_pos = mousePos;
        
        if (offset.x * offset.x + offset.y * offset.y > pan_threshold * pan_threshold)
        {
            common::view.move(offset);
        }
    }
    else if (common::sfml_event.type == sf::Event::MouseButtonReleased && common::sfml_event.mouseButton.button == sf::Mouse::Middle) 
    {
        is_panning = false;
    }
}
