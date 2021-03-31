#pragma once
#include "System.h"
#include "Enums.h"
// îò

class Tile
{
public:
	Tile() {};
	Tile(float x, float y, float tilesize, int tileType);											// pos (x,y),размер тайла, тип тайла
	virtual ~Tile() {};

	virtual void newTile(float x, float y, float tilesize, int tileType);		// новый тайл

	virtual void		setTileSize(v2f size);														// установить размер тайла
	const sf::Vector2f	getTileSize();																// вернуть размер тайла

	virtual void		setTilePosition(v2f pos);													// утсновить позицию тайла
	const sf::Vector2f	getTilePosition() const;													// вернуть позицию тайла

	virtual void		setTileType(int tileType);													// утсновить тип тайла
	const int			getTileType() const;														// вернуть тип тайла

	virtual void		setTileTexture(sf::Texture& texture);										// установить текстуру тайла

	virtual void		initNeighbours();															// устанавливаем как регировать на соседей
	virtual void		updateNeighbours();															// обновить соседей

	virtual void		update();																	// обновить
	virtual void		render(sf::RenderTarget& target);											// рендер
	int tileType;																					// тип тайла

protected:
private:
	sf::RectangleShape tile;																		// тайл
};

