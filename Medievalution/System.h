#pragma once
#include "Fonts.h"
#include "Audio.h"
#include "Resources.h"
#include "Input.h"
#include "RichText.h"

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

	/********************************************************/
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
	static Resources res;							// ресурсы
	static Input input;								// нажатия
	static sfe::RichText rText;						// цветной текст

	static unsigned short int gridSize;				// размер сетки карты
	static unsigned short int mapSize;				// размер карты

	static sf::Vector2i mousePosScreen;				// глобальное положение мыши на мониторе
	static sf::Vector2i mousePosWindow;				// положение мыши относительно окна приложения
	static sf::Vector2f mousePosView;				// положение мыши относительно камеры вида
	static sf::Vector2u mousePosGrid;				// полоэение мыши относительно сетки карты

	System();
};