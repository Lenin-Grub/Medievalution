#include "../stdafx.h"
#include "Camera.h"

//------------------------------------------------------------------------------------------------------------------------
// инициализация камеры
void Camera::initView()
{
	core::view.setSize(sf::Vector2f(settings.resolution.width, settings.resolution.height));
	core::view.setCenter(sf::Vector2f(settings.resolution.width / 2.f, settings.resolution.height / 2.f));
}

//------------------------------------------------------------------------------------------------------------------------
// обновление камеры
void Camera::updateView(const float& dtime)
{
	move(dtime);
}

//------------------------------------------------------------------------------------------------------------------------
// перемещение камеры
void Camera::move(const float& dtime)
{
	//движение камеры WASD
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { core::view.move(-settings.camera_speed * dtime, 0);}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { core::view.move(settings.camera_speed * dtime, 0);}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { core::view.move(0, -settings.camera_speed * dtime);}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { core::view.move(0, settings.camera_speed * dtime);}
}

//------------------------------------------------------------------------------------------------------------------------
// прокрутка камеры колесиком мышки
// to do  добавить проверку на GUI
void Camera::zoom()
{
	if (core::sfmlEvent.type == sf::Event::MouseWheelScrolled)
	{
		if (core::sfmlEvent.mouseWheelScroll.delta > 0)
		{
			if (core::view.getSize().x <= max_zoom || core::view.getSize().y <= max_zoom)
				core::view.zoom(1.1 + settings.zoom_speed);
		}
		else if (core::sfmlEvent.mouseWheelScroll.delta < 0)
		{
			if (core::view.getSize().x >= min_zoom || core::view.getSize().y >= min_zoom)
				core::view.zoom(0.9 - settings.zoom_speed);
		}
	}
}