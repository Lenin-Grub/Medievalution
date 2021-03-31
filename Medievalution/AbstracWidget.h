#pragma once
#include "System.h"
#include "Enums.h"
// îò
class AbstracWidget :
	public S
{
protected:
	Shape shape;
public:
	AbstracWidget(const Shape& shape) {};
	virtual ~AbstracWidget() = default;

	virtual const bool isWidgetPressed() { return false; };												// проверка на нажатие

	virtual void update(const sf::Vector2i& mousePosWindow) {};								// обновление
	virtual void render(sf::RenderTarget& target) { target.draw(shape); }					// отрисовка
};