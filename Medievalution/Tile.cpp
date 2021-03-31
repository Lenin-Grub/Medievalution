#include "stdafx.h"
#include "Tile.h"
// îò
Tile::Tile(float x, float y, float tilesize, int tileType)
{
	this->tile.setPosition(v2f(x, y));
	this->tile.setSize(v2f(tilesize, tilesize));
	this->setTileType(tileType);
	//cout << this->getTileType();
		switch (this->tileType)
{
case TILE_NONE:										//ничего
	break;
case TILE_WATER:									//вода
	tile.setTexture(&S::res.textureResources.useTexture("WATER"));
	break;
case TILE_SAND:										// песок
	tile.setTexture(&S::res.textureResources.useTexture("SAND"));
	break;
case TILE_GROUND:									// земля
	tile.setTexture(&S::res.textureResources.useTexture("DIRT"));
	break;
case TILE_GRASS:									// трава
	tile.setTexture(&S::res.textureResources.useTexture("GRASS"));
	break;
case TILE_ROCK:										// трава
	tile.setTexture(&S::res.textureResources.useTexture("ROCK"));
	break;
default:											// дефолтный
	std::cerr << "tile type not found" << endl;
	break;
}
}

void Tile::newTile(float x, float y, float tilesize,  int tileType)
{
	this->tileType = tileType;
	this->tile.setPosition(v2f(x, y));
	this->tile.setSize(v2f(tilesize, tilesize));
//	this->setTileTexture(texture);
}

void Tile::setTileSize(v2f size)
{
	this->tile.setSize(v2f(size.x, size.y));
}

const sf::Vector2f Tile::getTileSize()
{
	return this->tile.getSize();
}

void Tile::setTilePosition(v2f pos)
{
	this->tile.setPosition(v2f(pos.x, pos.y));
}

const sf::Vector2f Tile::getTilePosition() const
{
	return this->tile.getPosition();
}

void Tile::setTileType(int tileType)
{
	this->tileType = tileType;
}

const int Tile::getTileType() const
{
	return this->tileType;
}

void Tile::setTileTexture(sf::Texture& texture)
{
	this->tile.setTexture(&texture);
}

void Tile::initNeighbours()
{
	// думаем что делать с соседями
}

void Tile::updateNeighbours()
{
//	 обновляем реакцию на соседей
}

void Tile::update()
{
	 //обновляем логику карты
}

void Tile::render(sf::RenderTarget& target)
{
		target.draw(this->tile);
}
