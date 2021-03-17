#pragma once
#include "System.h"

enum TILE_TYPE
{
	TILE_NONE = 0,		// ничего
	TILE_WATER,			// вода
	TILE_SAND,			// песок
	TILE_GROUND,		// земля
	TILE_GRASS,			// трава
};

class Tile
{
private:
	sf::RectangleShape tile;								// тайл
public:
	short unsigned int tileType;							// тип тайла
//	Tile() {}
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
			this->tile.setTexture(&S::textures.Terrain[0]);
			break;
		case TILE_SAND:										// песок
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setTexture(&S::textures.Terrain[1]);
			break;
		case TILE_GROUND:									// земля
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setTexture(&S::textures.Terrain[2]);
			break;
		case TILE_GRASS:									// трава
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setTexture(&S::textures.Terrain[3]);
			break;
		default:											// дефолтный красный
			this->tile.setSize(v2f(tilesize, tilesize));
			this->tile.setPosition(x, y);
			this->tile.setFillColor(sf::Color::Red);
			break;
		}
	};
	~Tile() {};
	//void update(){}
	void render(sf::RenderTarget& target) { target.draw(this->tile); }
};

