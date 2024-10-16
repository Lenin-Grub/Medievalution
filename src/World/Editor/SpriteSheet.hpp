#pragma once
#include "../../Common/Common.h"
#include "Board.hpp"

class SpriteSheet 
    : public sf::Drawable
    , public sf::Transformable
{
public:
    SpriteSheet() = default;

    /// @param file_name The file name of the sprite sheet.
    /// @param tile_size The size of each tile in the sprite sheet.
    SpriteSheet(std::string file_name, int tile_size);

    ///	@brief Loads a texture from a file, calculates the # of columns and rows, retrieves
    /// board dimensions, resizes a vector to store 
    void import(const Board & board);
    
    ///	@brie Iterates through the vertex array setting the position of each quad.
    ///	Maps a texture to each quad based on the value of the tile 
    void mergeTiles();

    ///	@brie Updates the tile id at the given position. Xpos and Ypos are absolute coordinates
    ///	that need to be converted into relative coordinates to find the clicked tile
    /// @param id The tile ID that you want place.
    /// @param pos The coordinates of the tile position.
    void addTileId(int id, sf::Vector2f pos);

    /// @brief Returns a reference to the tileset texture.
    /// @return sf::Texture & The tileset texture.
    sf::Texture& getTilesetTexture();

    /// @brief Returns the size of each tile in the sprite sheet.
    /// @return int The tile size.
    const int getTileSize() const;

    /// @brief Returns the width of the sprite sheet.
    /// @return int The sheet width.
    const int getSheetWidth() const;

    /// @brief Returns the height of the sprite sheet.
    /// @return int The sheet height.
    const int getSheetHeight() const;

    int getTileCount() const;

    sf::Vector2u getTilePosition(int id) const;

private:
    sf::Texture      tileset_texture;
    sf::Vector2i     board_size;
    std::string      file_name;
    int              tile_size;
    int              tileset_cols;
    int              tileset_rows;
    int              board_tile_width;
    std::vector<int> tile_ids;
    sf::VertexArray  drawn_tiles;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};