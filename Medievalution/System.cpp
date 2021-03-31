#include "stdafx.h"
#include "System.h"
// îò
sf::Event				S::sfmlEvent;					// события sfml
sf::View				S::view;						// камера вида
float					S::dtime;						// время

Fonts					S::fonts;						// шрифты
Resources				S::res;							// ресурсы игры (текстуры, звуки, музыка, шрифты)
Input					S::input;						// нажатия игрока
sfe::RichText			S::rText;						// цветной текст

unsigned short int		S::gridSize = 32;				// размер сетки карты
unsigned short int		S::mapSize = 50;				// размер карты

sf::Vector2i			S::mousePosScreen;				// глобальное положение мыши на мониторе
sf::Vector2i			S::mousePosWindow;				// положение мыши относительно окна приложения
sf::Vector2f			S::mousePosView;				// положение мыши относительно камеры вида
sf::Vector2u			S::mousePosGrid;				// положение мыши относительно сетки карты

System::System()
{
	setlocale(LC_ALL, "ru");
}