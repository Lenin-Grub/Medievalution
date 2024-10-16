#include "../../stdafx.h"
#include "spritesheet.hpp"

SpriteSheet::SpriteSheet(std::string file_name, int tile_size)
    : file_name(file_name)
    , tile_size(tile_size)
    , board_size(sf::Vector2i(0,0))
    , board_tile_width(0)
    , tileset_cols(0)
    , tileset_rows(0)
{
}

void SpriteSheet::import(const Board & board)
{
    tileset_texture = ResourceLoader::instance().getTexture(file_name);

    tileset_cols = std::round(tileset_texture.getSize().x / tile_size);
    tileset_rows = std::round(tileset_texture.getSize().y / tile_size);

    board_size     = board.board_size;
    board_tile_width = board.tile_size;

    tile_ids.resize(static_cast<int64_t>(board_size.x * board_size.y), -1);
}

//TO DO replace to class Editor
void SpriteSheet::mergeTiles()
{
    // resize the vertex array to fit the level size
    drawn_tiles.setPrimitiveType(sf::Quads);
    drawn_tiles.resize(static_cast<int64_t>(board_size.x * board_size.y * 4));

    // populate the vertex array, with one quad per tile
    for (int i = 0; i < board_size.x; ++i)
    {
        for (int j = 0; j < board_size.y; ++j)
        {

            int id = tile_ids.at(static_cast<int64_t>(i + j * board_size.x));

            if (id != -1)
            {
                // get a pointer to the current tile's quad
                sf::Vertex* quad = &drawn_tiles[static_cast<int64_t>((i + j * board_size.x) * 4)];

                // calculate the tu and tv using the id and the number of columns and rows in the tileset
                int tu = id % tileset_cols;
                int tv = id / tileset_cols;

                // define its 4 corners
                quad[0].position = sf::Vector2f( i *      tile_size,  j      * tile_size);
                quad[1].position = sf::Vector2f((i + 1) * tile_size,  j      * tile_size);
                quad[2].position = sf::Vector2f((i + 1) * tile_size, (j + 1) * tile_size);
                quad[3].position = sf::Vector2f( i *      tile_size, (j + 1) * tile_size);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f( tu      * tile_size,  tv      * tile_size);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size,  tv      * tile_size);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size, (tv + 1) * tile_size);
                quad[3].texCoords = sf::Vector2f( tu      * tile_size, (tv + 1) * tile_size);
            }
        }
    }
}

//TO DO replace to class Editor
void SpriteSheet::addTileId(int id, sf::Vector2f pos)
{
    int x = static_cast<int>(pos.x / board_tile_width);
    int y = static_cast<int>(pos.y / board_tile_width);

    if (x >= 0 && x < board_size.x && y >= 0 && y < board_size.y)
    {
        int index = x + y * board_size.x;
        tile_ids.at(index) = id;
    }
}

sf::Texture& SpriteSheet::getTilesetTexture() 
{
    return tileset_texture;
}

const int SpriteSheet::getTileSize() const
{
    return tile_size;
}

const int SpriteSheet::getSheetWidth() const
{
    return tileset_cols;
}

const int SpriteSheet::getSheetHeight() const
{
    return tileset_rows;
}

int SpriteSheet::getTileCount() const
{
    return tileset_cols * tileset_rows;
}

sf::Vector2u SpriteSheet::getTilePosition(int id) const
{
    int tu = id % tileset_cols;
    int tv = id / tileset_cols;
    return sf::Vector2u(tu * tile_size, tv * tile_size);
}

void SpriteSheet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture    = &tileset_texture;
    target.draw(drawn_tiles, states);
}
