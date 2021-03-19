#include "stdafx.h"
#include "CheckBox.h"

CheckBox::~CheckBox()
{
}

const bool CheckBox::isWidgetPressed()
{
	if (this->buttonState == WIDGET_ACTIVE)
		return true;
	return false;
}

const int CheckBox::getActive() const
{
	return this->isActive;
}

void CheckBox::setActive(bool isActive, sf::Texture& texture)
{
	this->shape.setTexture(&texture);
	this->isActive = isActive;
}

void CheckBox::update(const sf::Vector2i& mousePosWindow)
{
		if (this->shape.getGlobalBounds().contains(static_cast <sf::Vector2f> (mousePosWindow)) && isMouseReleased(sf::Mouse::Left))	// проверяем попали ли мышью по кнопке
		{
			if (this->isActive == false)
			{
				this->buttonState = WIDGET_IDLE;													// кнопка нажата
				this->isActive =!this->isActive;
				cout << "idle " << this->getActive() << endl;
			}
			else
			{
				this->buttonState = WIDGET_ACTIVE;
				this->isActive = false;
				cout <<"active "<<this->getActive() << endl;
			}
		}


	switch (this->buttonState)																	// проверка состояний кнопок
	{
	case WIDGET_IDLE:																			// кнопка-бездельник
		this->shape.setFillColor(sf::Color::Green);
		this->shape.setTexture(&S::res.textureResources.useTexture("THUMB_UP"));
		break;
	case WIDGET_ACTIVE:																			// кнопка нажата
		this->shape.setFillColor(sf::Color::Red);
		this->shape.setTexture(&S::res.textureResources.useTexture("THUMB_DOWN"));
		break;
	default:																					// если что-то пошло не так
		this->shape.setFillColor(Color::Red);
		break;
	}
}

void CheckBox::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}
