#pragma once
#include "../../Core/Core.h"
#include "Board.hpp"

class SpriteSheet 
	: public sf::Drawable
	, public sf::Transformable
{
public:
	SpriteSheet();
	SpriteSheet(std::string file_name, int tile_size);

	/*
		Loads a texture from a file, calculates the # of columns and rows, retrieves
		board dimensions, resizes a vector to store tile IDs
	*/
	void import(Board & board);

	/*
		Iterates through the vertex array setting the position of each quad.
		Maps a texture to each quad based on the value of the tile id
	*/
	void mergeTiles();

	/*
		Updates the tile id at the given position. Xpos and Ypos are absolute coordinates
		that need to be converted into relative coordinates to find the clicked tile
	*/
	void addTileId(int id, int xpos, int ypos);

	sf::Texture& getTilesetTexture();

	const int getTileSize();
	const int getSheetWidth();
	const int getSheetHeight();

private:
	sf::Texture m_tilesetTexture;
	std::string m_file_name;
	int m_tile_size; // width/height of single tile in pixels
	int m_tileset_cols;
	int m_tileset_rows;

	int m_boardWidth;
	int m_boardHeight;
	int m_board_tileWidth;

	std::vector<int> m_tile_ids;
	sf::VertexArray m_drawn_tiles;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};