#include "../../stdafx.h"
#include "spritesheet.hpp"


SpriteSheet::SpriteSheet()
    : tile_size(0)
    , tileset_cols(0)
    , tileset_rows(0)
{
}

SpriteSheet::SpriteSheet(std::string file_name, int tile_size)
    : file_name(file_name)
    , tile_size(tile_size)
{
}

void SpriteSheet::import(Board & board)
{
    file_name = "./resources/" + file_name;

    if (!tileset_texture.loadFromFile(file_name))
    {
        LOG_ERROR("Texture map not loaded");
    }

    tileset_cols = std::round(tileset_texture.getSize().x / tile_size);
    tileset_rows = std::round(tileset_texture.getSize().y / tile_size);

    board_height = board.board_height;
    board_width = board.board_width;
    board_tile_width = board.tile_size;

    tile_ids.resize(board_width * board_height, -1);
}

void SpriteSheet::mergeTiles()
{
    // resize the vertex array to fit the level size
    drawn_tiles.setPrimitiveType(sf::Quads);
    drawn_tiles.resize(board_width * board_height * 4);

    // populate the vertex array, with one quad per tile
    for (int i = 0; i < board_width; ++i)
    {
        for (int j = 0; j < board_height; ++j)
        {

            int id = tile_ids.at(i + j * board_width);

            if (id != -1)
            {
                // get a pointer to the current tile's quad
                sf::Vertex* quad = &drawn_tiles[(i + j * board_width) * 4];

                // calculate the tu and tv using the id and the number of columns and rows in the tileset
                int tu = id % tileset_cols;
                int tv = id / tileset_cols;

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * 32, j * 32);
                quad[1].position = sf::Vector2f((i + 1) * 32, j * 32);
                quad[2].position = sf::Vector2f((i + 1) * 32, (j + 1) * 32);
                quad[3].position = sf::Vector2f(i * 32, (j + 1) * 32);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tile_size, tv * tile_size);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size, tv * tile_size);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size, (tv + 1) * tile_size);
                quad[3].texCoords = sf::Vector2f(tu * tile_size, (tv + 1) * tile_size);
            }
        }
    }
}

void SpriteSheet::addTileId(int id, sf::Vector2f pos)
{
    int tile_x_pos = pos.x / 32;
    int tile_y_pos = pos.y / 32;
    const int max_index = 1920;
    int index = tile_x_pos + tile_y_pos * board_width;

    if (index < max_index)
    {
        tile_ids.at(index) = id;
    }
}

sf::Texture& SpriteSheet::getTilesetTexture()
{
    return tileset_texture;
}

const int SpriteSheet::getTileSize()
{
    return tile_size;
}

const int SpriteSheet::getSheetWidth()
{
    return tileset_cols;
}

const int SpriteSheet::getSheetHeight()
{
    return tileset_rows;
}

void SpriteSheet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &tileset_texture;
    target.draw(drawn_tiles, states);
}
