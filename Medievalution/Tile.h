#pragma once
#include "System.h"
#include "Enums.h"


class Tile
{
public:
	short unsigned int tileType;							// тип тайла
	Tile(int x, int y, float tilesize, int tileType)		// pos (x,y),размер тайла, тип тайла
	{
		this->tileType = tileType;
		switch (this->tileType)
		{
		case TILE_NONE:										//ничего
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x,y);
			this->tile.setFillColor(sf::Color::Transparent);
				break;
		case TILE_WATER:									//вода
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setTexture(&S::res.textureResources.useTexture("WATER"));
			break;
		case TILE_SAND:										// песок
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setTexture(&S::res.textureResources.useTexture("SAND"));
			break;
		case TILE_GROUND:									// земля
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setTexture(&S::res.textureResources.useTexture("DIRT"));
			break;
		case TILE_GRASS:									// трава
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setTexture(&S::res.textureResources.useTexture("GRASS"));
			break;
		case TILE_ROCK:									// трава
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setTexture(&S::res.textureResources.useTexture("ROCK"));
			break;
		default:											// дефолтный красный
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setFillColor(sf::Color::Red);
			break;
		}
	};

	~Tile() {};

	void render(sf::RenderTarget& target)					// рендер
	{
		target.draw(this->tile); 
	}

private:
	sf::RectangleShape tile;								// тайл
};

