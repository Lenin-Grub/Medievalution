#pragma once
#include "System.h"

enum widget_states				// состо€ни€ виджетов
{
	WIDGET_IDLE = 0,			// кнопка без действи€	"бездельник"
	WIDGET_ACTIVE,				// нажата€ кнопка		"нажато"
	WIDGET_HOVER,				// мышь над кнопкой		"готово к нажатию"
	WIDGET_DISABLED,			// мышь не активна		"не доступна дл€ нажати€"
};

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