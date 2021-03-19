#pragma once
#include "Tile.h"

class Map
{
private:
	v2u maxSize;																			// карта размера
	std::vector<std::vector<std::vector<std::shared_ptr<Tile>>>> map;						// карта			(x,y,tile)
	int gridSizeF;																			// размер сетки float
	unsigned gridSizeU;																		// размер сетки unsigned
	int layer = 1;																			// текущий слой
	int tileType;																			// тип тайла

public:
	Map(float gridSize, unsigned width, unsigned height, int tileType)
	{
		this->maxSize.x = width;
		this->maxSize.y = height;
		this->gridSizeF = gridSize;
		this->gridSizeU = static_cast<unsigned>(this->gridSizeF);
		this->tileType = tileType;

		map.resize(this->maxSize.x,std::vector<std::vector<shared_ptr<Tile>>>());
		for (size_t x = 0; x < maxSize.x; x++)
		{
			for (size_t y = 0; y < maxSize.y; y++)
			{
				map[x].resize(this->maxSize.y,std::vector<std::shared_ptr<Tile>>());
				for (size_t z = 0; z < this->layer; z++)
				{
					map[x][y].resize(this->layer, std::make_unique<Tile>(x * this->gridSizeF, y * this->gridSizeF, this->gridSizeF, rand() % 5 + 1));
				}
			}
		}

	}
	~Map()
	{
		if (!this->map.empty())
		{
			for (int x = 0; x < this->map.size(); x++)
			{
				for (int y = 0; y < this->map[x].size(); y++)
				{
					for (int z = 0; z < this->map[x][y].size(); z++)
					{
						this->map[x][y][z] = NULL;

					}
					this->map[x][y].clear();
				}
				this->map[x].clear();
			}
			this->map.clear();
		}
	}

	void addTile(const unsigned x, const unsigned y, const unsigned z, int tileType)
	{
		if (x < this->maxSize.x && x >= 0 &&
			y < this->maxSize.y && y >= 0 &&
			z <= this->layer && z >= 0)
		{
			if (this->map[x][y][z] != NULL)
			{
				//delete this->map[x][y][z];
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

	const int getTileType() const{		// возвращает тип тайла
		return this->tileType;
	}

	void setTileType(int tileType)	{	// устанавливает тип тайла
		this->tileType = tileType;
	}

	void removeTile(const unsigned x, const unsigned y, const unsigned z)
	{
		if (x < this->maxSize.x && x >= 0 &&
			y < this->maxSize.y && y >= 0 &&
			z <= this->layer && z >= 0)
		{
			if (this->map[x][y][z] != NULL)
			{
				//delete this->map[x][y][z];
				this->map[x][y][z] = NULL;
			}
		}
	}


	void render(sf::RenderTarget& target)	// отрисовка карты
	{
		for (auto& x: this->map)
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
};

