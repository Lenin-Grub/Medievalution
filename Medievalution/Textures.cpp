#include "stdafx.h"
#include "Textures.h"
// îò

sf::Texture& Textures::useTexture(const std::string& texture)					// вызов текстуры
{
	return this->textures.at(texture);											// возвращаем текстуру по запросу
}

void Textures::loadTexture(const std::string& name, const std::string& file)	// загрузка текстуры из файла
{
	sf::Texture texture;

	texture.loadFromFile("Resourses/" + file);									// загружаем текстуру
	this->textures[name] = texture;												// даем текстуре имя

	return;
}

Textures::Textures(){}
Textures::~Textures(){}