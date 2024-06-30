#include "../stdafx.h"
#include "Camera.h"

Camera::Camera()
	: max_zoom		(1500)
	, min_zoom		(90)
	, pan_threshold (5.0f)
	, is_panning	(false)
{
	core::view.setSize(sf::Vector2f(WindowSettings::getInstance().resolution.width, 
									WindowSettings::getInstance().resolution.height));

	core::view.setCenter(sf::Vector2f(WindowSettings::getInstance().resolution.width	/ 2.f, 
									  WindowSettings::getInstance().resolution.height	/ 2.f));
}

Camera::~Camera()
{
}

void Camera::update(const float& dtime)
{
	move(dtime);
}

void Camera::move(const float& dtime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { core::view.move(-WindowSettings::getInstance().camera_speed * dtime, 0); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { core::view.move(WindowSettings::getInstance().camera_speed * dtime, 0);  }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { core::view.move(0, -WindowSettings::getInstance().camera_speed * dtime); }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { core::view.move(0, WindowSettings::getInstance().camera_speed * dtime);  }

}

//TODO  добавить проверку на GUI
void Camera::zoom()
{
	if (core::sfml_event.type == sf::Event::MouseWheelScrolled)
	{
		if (core::sfml_event.mouseWheelScroll.delta > 0)
		{
			if (core::view.getSize().x <= max_zoom || core::view.getSize().y <= max_zoom)
				core::view.zoom(1.1 + WindowSettings::getInstance().zoom_speed);
		}
		else if (core::sfml_event.mouseWheelScroll.delta < 0)
		{
			if (core::view.getSize().x >= min_zoom || core::view.getSize().y >= min_zoom)
				core::view.zoom(0.9 - WindowSettings::getInstance().zoom_speed);
		}
	}
}

void Camera::scroll()
{
	if (core::sfml_event.type == sf::Event::MouseButtonPressed && core::sfml_event.mouseButton.button == sf::Mouse::Middle) 
	{
		prev_mouse_pos.x = core::mouse_pos_window.x;
		prev_mouse_pos.y = core::mouse_pos_window.y;
		is_panning = true;
	}
	else if (core::sfml_event.type == sf::Event::MouseMoved && is_panning) 
	{
		sf::Vector2f mousePos;
		mousePos.x = core::mouse_pos_window.x;
		mousePos.y = core::mouse_pos_window.y;
		
		sf::Vector2f offset = prev_mouse_pos - mousePos;
		prev_mouse_pos = mousePos;
		
		if (offset.x * offset.x + offset.y * offset.y > pan_threshold * pan_threshold) 
		{
			core::view.move(offset);
		}
	}
	else if (core::sfml_event.type == sf::Event::MouseButtonReleased && core::sfml_event.mouseButton.button == sf::Mouse::Middle) 
	{
		is_panning = false;
	}
}
