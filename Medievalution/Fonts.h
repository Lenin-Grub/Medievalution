#pragma once
#include "Define.h"
// îò
struct Fonts																	//шрифты
{
	sf::Font _font;																// Переменные типа шрифт, которые передают в текст в качестве аргумента


	Fonts()																		//загрузка шрифта
	{
		if (!_font.loadFromFile("Resourses/Fonts/OpenSans-Semibold.ttf"))
			cout << "ERROR: font not found!" << endl;
		else
			cout << "Resourses:: Fonts:: font _font loaded" << endl;
	}

	virtual ~Fonts() {}
};