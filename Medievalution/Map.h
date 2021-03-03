#pragma once
#include "Tile.h"

class Map
{
private:
	v2u maxSize;
	std::vector<std::vector<std::vector<Tile*>>> map;
	int gridSizeF;										
	unsigned gridSizeU;
	int layer = 1;
	int tileType;

public:
	Map(float gridSize, unsigned width, unsigned height, int tileType)
	{
		this->maxSize.x = width;
		this->maxSize.y = height;
		this->gridSizeF = gridSize;
		this->gridSizeU = static_cast<unsigned>(this->gridSizeF);
		this->tileType = tileType;

		map.resize(this->maxSize.x, std::vector<std::vector<Tile*>>());
		for (size_t x = 0; x < this->maxSize.y; x++)
		{
			for (size_t y = 0; y < this->maxSize.x; y++)
			{
				map[x].resize(this->maxSize.y, std::vector<Tile*>());
				for (size_t z = 0; z < this->layer; z++)
				{
					map[x][y].resize(this->layer, new Tile(x * this->gridSizeF, y * this->gridSizeF,this->gridSizeF,rand()%4+1));
				}
			}
		}

	}
	~Map()
	{
		for (size_t x = 0; x < this->maxSize.x; x++)
		{
			for (size_t y = 0; y < this->maxSize.y; y++)
			{
				for (size_t z = 0; z < this->layer; z++)
				{
					delete this->map[x][y][z];
				}
			}
		}
	}

	void addTile(const unsigned x, const unsigned y, const unsigned z, int tileType)
	{

		//удаляем тайл
		if (x < this->maxSize.x && x >= 0 &&
			y < this->maxSize.y && y >= 0 &&
			z <= this->layer && z >= 0)
		{
			if (this->map[x][y][z] != NULL)
			{
				delete this->map[x][y][z];
				this->map[x][y][z] = NULL;
			}
		}
		// добавляем тайл
		if (x < this->maxSize.x && x >= 0 &&
			y < this->maxSize.y && y >= 0 &&
			z <= this->layer && z >= 0)
		{
			if (this->map[x][y][z] == NULL)
			{
				this->map[x][y][z] = new Tile(x * this->gridSizeF, y * this->gridSizeF, this->gridSizeF,this->tileType);
			}
		}
	}

	const int getTileType() const{		// возвращает тип тайла
		return this->tileType;
	}

	void setTileType(int tileType)	{	// устанавливает тип тайла
		this->tileType = tileType;
	}

	void removeTile(const unsigned x, const unsigned y, const unsigned z)
	{
		// удаляем тайл
		if (x < this->maxSize.x && x >= 0 &&
			y < this->maxSize.y && y >= 0 &&
			z <= this->layer && z >= 0)
		{
			if (this->map[x][y][z] != NULL)
			{
				delete this->map[x][y][z];
				this->map[x][y][z] = NULL;
			}
		}
	}


	void render(sf::RenderTarget& target)	// отрисовка карты
	{
		for (auto& x : this->map)
		{
			for (auto& y : x)
			{
				for (auto* z : y)
				{
					if (z != NULL)
						z->render(target);
				}
			}
		}
	}
};

