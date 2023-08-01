#include "../stdafx.h"
#include "Camera.h"

void Camera::initView()
{
	core::view.setSize(sf::Vector2f(WindowSettings::getInstance().resolution.width, WindowSettings::getInstance().resolution.height));
	core::view.setCenter(sf::Vector2f(WindowSettings::getInstance().resolution.width / 2.f, WindowSettings::getInstance().resolution.height / 2.f));
}

void Camera::updateView(const float& dtime)
{
	move(dtime);
}

void Camera::move(const float& dtime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { core::view.move(-WindowSettings::getInstance().camera_speed * dtime, 0);}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { core::view.move(WindowSettings::getInstance().camera_speed * dtime, 0);}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { core::view.move(0, -WindowSettings::getInstance().camera_speed * dtime);}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { core::view.move(0, WindowSettings::getInstance().camera_speed * dtime);}
}

//TODO  добавить проверку на GUI
void Camera::zoom()
{
	if (core::sfmlEvent.type == sf::Event::MouseWheelScrolled)
	{
		if (core::sfmlEvent.mouseWheelScroll.delta > 0)
		{
			if (core::view.getSize().x <= max_zoom || core::view.getSize().y <= max_zoom)
				core::view.zoom(1.1 + WindowSettings::getInstance().zoom_speed);
		}
		else if (core::sfmlEvent.mouseWheelScroll.delta < 0)
		{
			if (core::view.getSize().x >= min_zoom || core::view.getSize().y >= min_zoom)
				core::view.zoom(0.9 - WindowSettings::getInstance().zoom_speed);
		}
	}
}