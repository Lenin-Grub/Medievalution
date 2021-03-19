#pragma once
#include "AbstracWidget.h"
#include "Button.h"

class CheckBox :
	public AbstracWidget
{
public:
	CheckBox(float x, float y, float width, float height, sf::Texture& texture) :AbstracWidget(shape) {
		this->shape.setPosition(v2f(x, y));
		this->shape.setSize(v2f(width, height));

		this->shape.setTexture(&texture);
		this->isActive = false;
	};
    virtual ~CheckBox();

	const bool isWidgetPressed() override;															// нажата ли кнопка?

	const int getActive() const;														// возвращает активна ли кнопка
	void setActive(bool isActive, sf::Texture& texture);											// устанавливает кнопку активной;

	void update(const sf::Vector2i& mousePosWindow)	override;										// обновление
	void render(sf::RenderTarget& target) override;													// отрисовка
    
private:
	short unsigned buttonState;

	bool isActive;							// активен ли список

	sf::Color idleColor;					// цвет в состоянии "бездельник"
	sf::Color hoverColor;					// цвет в состоянии "готово к нажатию"
	sf::Color activeColor;					// цвет в состоянии "нажато"

	sf::Color outLineIdleColor;				// обвокдка в состоянии "бездельник"
	sf::Color outLineHoverColor;			// обвокдка в состоянии "готово к нажатию"
	sf::Color outLineActiveColor;			// обвокдка в состоянии "нажато"
};

