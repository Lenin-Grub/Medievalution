#pragma once
#include "Fonts.h"
#include "Textures.h"
#include "Audio.h"


class System
{
public:
	/**************************************/
	/*создание объекта Shape и перегрузки*/
	static const Shape сreateShape(sf::Vector2f pos, sf::Vector2f siz, sf::Texture& png)						//v2f x.y, размер, текстура
	{
		Shape shp;
		shp.setOrigin(siz / 2.f);
		shp.setSize(siz);
		shp.setPosition(pos);
		shp.setTexture(&png);
		return shp;
	}

	static const Shape сreateShape(sf::Vector2f pos, sf::Vector2f siz)											//v2f x.y, рамзер
	{
		Shape shp;
		shp.setOrigin(siz / 2.f);
		shp.setSize(siz);
		shp.setPosition(pos);
		return shp;
	}

	static const Shape сreateShape(sf::Vector2f pos, sf::Vector2f siz, sf::Color color)							//v2f x.y, размер, цвет
	{
		Shape shp;
		shp.setSize(siz);
		shp.setOrigin(siz / 2.f);
		shp.setFillColor(color);
		shp.setPosition(pos);
		return shp;
	}

	static const Shape сreateShape(sf::Vector2f pos, sf::Vector2f siz, float out_siz,
		sf::Color color = sf::Color::White, sf::Color out_color = sf::Color::White)	//v2f x.y, размер, толщина обводки, цвет, цвет обводки(белый по-умолчанию)
	{
		Shape shp;
		shp.setSize(siz);
		shp.setOrigin(siz / 2.f);
		shp.setFillColor(color);
		shp.setPosition(pos);
		shp.setOutlineThickness(out_siz);
		shp.setOutlineColor(out_color);
		return shp;
	}

	/********************************************************/
	/*создание текста (позиция, размер, текст, шрифт, цвет)*/
	static const sf::Text сreateText(v2f pos, uint siz, sf::String str, sf::Font& font, Color col)				//v2f x.y, размер, текст, шрифт, цвет
	{
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(siz);
		text.setFillColor(col);
		text.setString(str);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(pos.x, pos.y - siz / 3);
		return text;
	}

	static void сenteringText(sf::Text& text)		//центрирование текста , использовать после инициализации текста в строке
	{
		text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 1.5f);
	}

	/**************************************************************/
	/*функции для удобства вызова событий sfml. Клавиатуры и мыши*/
	virtual bool isKeyPressed(const sf::Keyboard::Key& code) {													//была ли нажата кнопка мыши?
		if (sfmlEvent.type == sf::Event::KeyPressed)
			if (sfmlEvent.key.code == code) return true;
		return false;
	}

	virtual bool isKeyReleased(const sf::Keyboard::Key& code) {													//была ли отжата кнопка клавиатуры
		if (sfmlEvent.type == sf::Event::KeyReleased)
			if (sfmlEvent.key.code == code) return true;
		return false;
	}

	virtual bool isMouseReleased(const sf::Mouse::Button& code) {												//была ли отжата кнопка мыши
		if (sfmlEvent.type == sf::Event::MouseButtonReleased)
			if (sfmlEvent.key.code == code) return true;
		return false;
	}

	virtual bool isMousePressed(const sf::Mouse::Button& code) {												//была ли зажата кнопка мыши?
		if (sfmlEvent.type == sf::Event::MouseButtonPressed)
			if (sfmlEvent.key.code == code) return true;
		return false;
	}

	virtual bool isMouseWheelUp(const sf::Mouse::Wheel code) {													//было ли нажато(прокручено) колесико мышки?
		if (sfmlEvent.mouseWheel.x == code) return true;
		return false;
	}

	virtual bool isMouseDoubleReleased(const sf::Mouse::Button& code)											// двойной клик
	{ 
		float timer;
		if (sfmlEvent.type == sf::Event::MouseButtonReleased && sfmlEvent.key.code == code)
		{
			timer = clock.getElapsedTime().asMilliseconds();
			if (sfmlEvent.type == sf::Event::MouseButtonReleased && sfmlEvent.key.code == code && timer <= 300.0f)
			{
				cout << "click!\n";
				return true;
			}
			else {
				cout << "ooops!\n";
				clock.restart();
				return false;
			}
		}
	}

	static const sf::FloatRect getFieldofView()						//возвращает поле видимости камеры
	{
		return sf::FloatRect(
			view.getCenter().x - (view.getCenter().x / 2),			//верхний левый угол по х
			view.getCenter().y - (view.getCenter().y / 2),			//верхний левый угол по у
			view.getSize().x, view.getSize().y						//нижний правый угол по x,y
		);
	}


public:
	static sf::Event sfmlEvent;						// события sfml
	static sf::View view;							// камера вида
	static float dtime;								// время

	static Fonts fonts;								// шрифты
	static Textures textures;						// текстуры
	static Audio audio;								// звуки

	static unsigned short int gridSize;				// размер сетки карты
	static unsigned short int mapSize;				// размер карты

	static sf::Vector2i mousePosScreen;				// глобальное положение мыши на мониторе
	static sf::Vector2i mousePosWindow;				// положение мыши относительно окна приложения
	static sf::Vector2f mousePosView;				// положение мыши относительно камеры вида
	static sf::Vector2u mousePosGrid;				// полоэение мыши относительно сетки карты

	System();

private:
	static sf::Clock clock;							// время
};