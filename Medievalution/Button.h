#pragma once
#include "AbstractButton.h"

enum widget_states				// состояния виджетов
{
	WIDGET_IDLE = 0,			// кнопка без действия	"бездельник"
	WIDGET_ACTIVE,				// нажатая кнопка		"нажато"
	WIDGET_HOVER,				// мышь над кнопкой		"готово к нажатию"
};


class Button
	:public AbstractButton
{
private:
	short unsigned buttonState;
//	short unsigned id;
	sf::Text text;

	sf::Color textIdleColor;				// текст в состоянии "бездельник"
	sf::Color textHoverColor;				// текст в состоянии "готово к нажатию"
	sf::Color textActiveColor;				// текст в состоянии "нажато"

	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;

	sf::Color outLineIdleColor;
	sf::Color outLineHoverColor;
	sf::Color outLineActiveColor;
public:
	Button(float x, float y, float width, float height, sf::Texture& texture) :AbstractButton(shape) 
	{
		this->shape.setPosition(v2f(x, y));
		this->shape.setSize(v2f(width, height));

		this->shape.setTexture(&texture);

		this->idleColor = Color::White;
		this->hoverColor = Color::Green;
		this->activeColor = Color::Blue;
	}

	Button(float x, float y, float width, float height, string text, int uinit ,sf::Font& font) :AbstractButton(shape)
	{
		this->shape.setPosition(v2f(x, y));
		this->shape.setSize(v2f(width, height));

		this->text.setFont(font);
		this->text.setString(text);
		this->text.setCharacterSize(uinit);
		this->text.setPosition(
			this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
			this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f);

		this->idleColor = Color::White;
		this->hoverColor = Color::Green;
		this->activeColor = Color::Blue;

		this->textIdleColor = Color::Black;
		this->textHoverColor = Color::Black;
		this->textActiveColor = Color::White;
	}

	virtual ~Button() { cout << "button destroed" << endl; }

	const bool isWidgetPressed() override																	// нажата ли кнопка?
	{
		if (this->buttonState == WIDGET_ACTIVE)
			return true;
		return false;
	}

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
			this->text.setFillColor(this->textIdleColor);
			break;

		case WIDGET_HOVER:																			// кнопка готова к нажатию
			this->shape.setFillColor(this->hoverColor);
			this->text.setFillColor(this->textHoverColor);
			break;

		case WIDGET_ACTIVE:																			// кнопка нажата
			this->shape.setFillColor(this->activeColor);
			this->text.setFillColor(this->textActiveColor);
			break;

		default:																					// если что-то пошло не так
			this->shape.setFillColor(Color::Red);
			this->text.setFillColor(Color::White);
			break;
		}
	}
	void render(sf::RenderTarget& target) override 
	{
		target.draw(shape);
		target.draw(text);
	}								// рендер
};