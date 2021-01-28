#pragma once
#include "Define.h"

class Textures
{
protected:
	sf::Texture Load(const std::string file, bool isSmooth = false)						//загрузка текстуры, по умолчанию сглаживание отключено
	{
		sf::Texture texture;
		texture.loadFromFile("Resourses/" + file);
		return texture;
	}

public:																					//добававлять текстуры тут
	vector <Img> GUIcons;

	vector<Img> Terrain;																//текстуры группы terrain
	vector<Img> Grass;																	//текстуры группы grass и тд

	Textures()																			//загружать текстуры здесь /папка/название + .формат
	{
		sf::Context context;
		for (int i = 0; i < 4; i++)
		{
			Terrain.push_back(Load("Tiles/terrain" + std::to_string(i) + ".png"));
			cout << "Resourses:: Tiles:: texture terrain " << std::to_string(i) << " loaded" << endl;
		}
		GUIcons.push_back(Load("Icons/exit-door.png"));
		GUIcons.push_back(Load("Icons/dig-dug.png"));
		GUIcons.push_back(Load("Icons/dynamite.png"));
	}

	~Textures() {};
};