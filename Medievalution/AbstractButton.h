#pragma once
#include "System.h"

enum widget_states				// состояния виджетов
{
	WIDGET_IDLE = 0,			// кнопка без действия	"бездельник"
	WIDGET_ACTIVE,				// нажатая кнопка		"нажато"
	WIDGET_HOVER,				// мышь над кнопкой		"готово к нажатию"
};

class AbstractButton
	:public S
{
protected:
	Shape shape;
	string id;
public:
	AbstractButton(const Shape& shape) {};//:shape(shape),id(id) {}
	virtual ~AbstractButton() = default;

	virtual const bool isWidgetPressed() { cout << "нажата" << endl; return true; }										// проверка на нажатие
	
//	virtual const short unsigned& getId() { return 0; }								// возвращает id кнопки
//	virtual short unsigned setId(const short unsigned id) { this->id = id; return id; }						// устанавливает id кнопки

	virtual void update(const sf::Vector2i& mousePosWindow) {}					// обновление
	virtual void render(sf::RenderTarget& target) { target.draw(shape); }								// отрисовка
};