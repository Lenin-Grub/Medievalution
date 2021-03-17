#include "stdafx.h"
#include "Define.h"


//перегрузка вывода v2f
std::ostream& operator << (std::ostream& out, const sf::Vector2f& p)
{
	out << "X: " << p.x << " | Y: " << p.y;
	return out;
}
//перегрузка вывода v2i
std::ostream& operator << (std::ostream& out, const sf::Vector2i& p)
{
	out << "X: " << p.x << " | Y: " << p.y;
	return out;
}
//перегрузка вывода v2u
std::ostream& operator << (std::ostream& out, const sf::Vector2u& p)
{
	out << "X: " << p.x << " | Y: " << p.y;
	return out;
}
//перегрузка вывода объекта shape (координаты, размер, цвет RGB)
std::ostream& operator << (std::ostream& out, const Shape& s)
{
	out << "Position: " << s.getPosition() << std::endl;
	out << "Size: " << s.getSize() << std::endl;
	const auto& c = s.getFillColor();
	out << "Color: R = " << c.r << " G = " << c.g << " B = " << c.b << std::endl;
	return out;
}