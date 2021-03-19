#pragma once
#include "Define.h"

class Textures
{
public:																					
	sf::Context context;																// важная хрень чтобы sfml не ругался о загрузке текстур раньше чем инициализированно окно
	Textures();																			// загружать текстуры здесь /папка/название + .формат
	virtual ~Textures();

	sf::Texture& useTexture(const std::string& texture);								// использовать текстуру для entities (вместо sf::Texture::setTexture())
	void loadTexture(const std::string& file, const std::string& name);					// загрузка текстуры, по умолчанию сглаживание отключено
private:
	std::map<std::string, sf::Texture> textures;										// карта текстур (имя текстуры, путь до файла текстуры)
};