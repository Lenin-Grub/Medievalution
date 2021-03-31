#pragma once
#include "Tile.h"
// îò
class Map
{
public:
	Map(float gridSize, unsigned width, unsigned height);
	virtual ~Map();

	void initMap();																							// инициализация карты

	//template <typename T>
	//void newTile(T);

	const int getTileType() const;																			// получить тип тайла
	void setTileType(int tileType);																			// установить тип тайла

	void addTile(const unsigned x, const unsigned y, const unsigned z, int tileType);						// добавить тайл
	void removeTile(const unsigned x, const unsigned y, const unsigned z);									// удалить тайл

	void update();																							// обновление карты
	void render(sf::RenderTarget& target);																	// отрисовка карты
protected:
	int tileType;																							// тип тайла
//	std::vector<Tile> tile;
private:
	v2u maxSize;																							// карта размера
	std::vector<std::vector<std::vector<std::shared_ptr<Tile>>>> map;										// карта (x,y,tile)
	int gridSizeF;																							// размер сетки float
	unsigned gridSizeU;																						// размер сетки unsigned
	int layer = 1;																							// текущий слой

	float x;
	float y;
};

