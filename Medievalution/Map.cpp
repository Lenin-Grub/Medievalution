#include "stdafx.h"
#include "Map.h"

Map::Map(float gridSize, unsigned width, unsigned height)
{
	this->maxSize.x = width;
	this->maxSize.y = height;
	this->gridSizeF = gridSize;
	this->gridSizeU = static_cast<unsigned>(this->gridSizeF);
	//this->tileType = (rand() % 5 + 1);
	//initMap();
}

Map::~Map()
{
	if (!this->map.empty())
	{
		for (int x = 0; x < this->map.size(); x++)
		{
			for (int y = 0; y < this->map[x].size(); y++)
			{
				for (int z = 0; z < this->map[x][y].size(); z++)
				{
					this->map[x][y][z].reset();
					this->map[x][y][z] = NULL;
				}
				this->map[x][y].clear();
			}
			this->map[x].clear();
		}
		this->map.clear();
	}
}

void Map::initMap()
{
	map.resize(this->maxSize.x, std::vector<std::vector<shared_ptr<Tile>>>());
	for (size_t x = 0; x < maxSize.x; x++)
	{
		for (size_t y = 0; y < maxSize.y; y++)
		{
			map[x].resize(this->maxSize.y, std::vector<std::shared_ptr<Tile>>());
			for (size_t z = 0; z < this->layer; z++)
			{
				map[x][y].resize(this->layer, std::make_unique<Tile>(x * this->gridSizeF, y * this->gridSizeF, this->gridSizeF, rand() % 5 + 1));
		/*		this->newTile(tile.push_back(std::make_unique<Tile>(x * this->gridSizeF, y * this->gridSizeF, this->gridSizeF, rand() % 5 + 1)));*/
			}
		}
	}
}


const int Map::getTileType() const
{
	return this->tileType;
}

void Map::setTileType(int tileType)
{
	this->tileType = tileType;
}

void Map::addTile(const unsigned x, const unsigned y, const unsigned z, int tileType)
{
	if (x < this->maxSize.x && x >= 0 &&
		y < this->maxSize.y && y >= 0 &&
		z <= this->layer && z >= 0)
	{
		if (this->map[x][y][z] != NULL)
		{
			this->map[x][y][z] = NULL;
		}
	}

	if (x < this->maxSize.x && x >= 0 &&
		y < this->maxSize.y && y >= 0 &&
		z <= this->layer && z >= 0)
	{
		if (this->map[x][y][z] == NULL)
		{
			this->map[x][y][z] = std::make_unique<Tile>(x * this->gridSizeF, y * this->gridSizeF, this->gridSizeF, tileType);
		}
	}
}

void Map::removeTile(const unsigned x, const unsigned y, const unsigned z)
{
	if (x < this->maxSize.x && x >= 0 &&
		y < this->maxSize.y && y >= 0 &&
		z <= this->layer && z >= 0)
	{
		if (this->map[x][y][z] != NULL)
		{
			this->map[x][y][z].reset();
			this->map[x][y][z] = NULL;
		}
	}
}

void Map::render(sf::RenderTarget& target)
{
		for (auto& x : this->map)
		{
			for (auto& y : x)
			{
				for (auto& z : y)
				{
					if (z != NULL)
						z->render(target);
				}
			}
		}
}
