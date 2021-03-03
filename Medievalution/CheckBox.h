#pragma once
#include "AbstractButton.h"

class CheckBox:
	public AbstractButton
{
private:
	short unsigned buttonState;		// состояние кнопки
	bool isActiveSignal;			// активен ли чекбокс

	sf::Color idleColor;			// цвет в состоянии "бездельник"
	sf::Color hoverColor;			// цвет в состоянии "готово к нажатию"
	sf::Color activeColor;			// цвет в состоянии "нажато"

public:
	CheckBox() 
		:AbstractButton(shape)
	{
		this->isActiveSignal = false;
	}
	~CheckBox(){}

	const bool isWidgetPressed() override				// нажата ли кнопка?
	{
		if (this->buttonState == WIDGET_ACTIVE)
			return true;
		return false;
	}

	//getActiveSignal
	//setActiveSignal

	void update(const sf::Vector2i& mousePosWindow)	override										// обновление
	{
		this->buttonState = WIDGET_IDLE;															// по-умолчанию кнопка бездельник
		if (this->shape.getGlobalBounds().contains(static_cast <sf::Vector2f> (mousePosWindow)))	// проверяем попали ли мышью по кнопке
		{
			this->buttonState = WIDGET_HOVER;														// есои прпали, кнопка готова к нажатию
			if (isMouseReleased(sf::Mouse::Left))													// проверка на нажатие ЛКМ
				this->buttonState = WIDGET_ACTIVE;													// кнопка нажата
		}

		switch (this->buttonState)																	// проверка состояний кнопок
		{
		case WIDGET_IDLE:																			// кнопка-бездельник
			this->shape.setFillColor(this->idleColor);
			break;

		case WIDGET_HOVER:																			// кнопка готова к нажатию
			this->shape.setFillColor(this->hoverColor);
			break;

		case WIDGET_ACTIVE:																			// кнопка нажата
			this->shape.setFillColor(this->activeColor);
			break;

		default:																					// если что-то пошло не так
			this->shape.setFillColor(Color::Red);
			break;
		}
	}
	void render(sf::RenderTarget& target) override													// рендер
	{
		target.draw(shape);
	}
};