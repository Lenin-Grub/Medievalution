#include "stdafx.h"
#include "Layer.hpp"

Layer::Layer(int tileSize, sf::Vector2i board_size, sf::Texture& texture)
    : tile_size(tileSize)
    , layer_size (board_size)
    , tileset_texture(texture)
    , tileset_cols(0)
    , tileset_rows(0)
    , visible(true)
{
}

bool Layer::init()
{
    try
    {
        tile_map.setPrimitiveType(sf::Quads);

        for (int x = 0; x < layer_size.x; x++)
        {
            for (int y = 0; y < layer_size.y; y++)
            {
                sf::Vector2f topLeft    ( y      * tile_size,  x      * tile_size);
                sf::Vector2f topRight   ((y + 1) * tile_size,  x      * tile_size);
                sf::Vector2f bottomRight((y + 1) * tile_size, (x + 1) * tile_size);
                sf::Vector2f bottomLeft  (y      * tile_size, (x + 1) * tile_size);

                tile_map.append(sf::Vertex(topLeft,     sf::Vector2f(0, 0)));
                tile_map.append(sf::Vertex(topRight,    sf::Vector2f(tile_size, 0)));
                tile_map.append(sf::Vertex(bottomRight, sf::Vector2f(tile_size, tile_size)));
                tile_map.append(sf::Vertex(bottomLeft,  sf::Vector2f(0, tile_size)));
            }
        }

        tileset_cols = std::round(tileset_texture.getSize().x / tile_size);
        tileset_rows = std::round(tileset_texture.getSize().y / tile_size);
        tile_ids.resize(static_cast<int64_t>(layer_size.x * layer_size.y), -1);

        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}


void Layer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &tileset_texture;
    target.draw(tile_map, states);
}

void Layer::addTile(const int& id, sf::Vector2f pos)
{
    if (id < -1 || id >= tileset_cols * tileset_rows)
    {
        LOG_ERROR("Tile ID: {0} is out of range", id);
        return;
    }

    int x = static_cast<int>(pos.x / tile_size);
    int y = static_cast<int>(pos.y / tile_size);

    if (x >= 0 && x < layer_size.x && y >= 0 && y < layer_size.y)
    {
        int index = x + y * layer_size.x;
        tile_ids.at(index) = id;

        sf::Vertex* quad = &tile_map[static_cast<int64_t>(index * 4)];

        if (id == -1)
        {
            // Transparent Tile: Set Texture Coordinates Outside Texture
            quad[0].texCoords = sf::Vector2f(0, 0);
            quad[1].texCoords = sf::Vector2f(0, 0);
            quad[2].texCoords = sf::Vector2f(0, 0);
            quad[3].texCoords = sf::Vector2f(0, 0);
        }
        else
        {
            // Visible tile coords
            int tu = id % tileset_cols;
            int tv = id / tileset_cols;

            quad[0].texCoords = sf::Vector2f( tu      * tile_size,  tv      * tile_size);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size,  tv      * tile_size);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size, (tv + 1) * tile_size);
            quad[3].texCoords = sf::Vector2f( tu      * tile_size, (tv + 1) * tile_size);
        }

        quad[0].position = sf::Vector2f( x      * tile_size,  y      * tile_size);
        quad[1].position = sf::Vector2f((x + 1) * tile_size,  y      * tile_size);
        quad[2].position = sf::Vector2f((x + 1) * tile_size, (y + 1) * tile_size);
        quad[3].position = sf::Vector2f( x      * tile_size, (y + 1) * tile_size);
    }
}

void Layer::removeTile(const int& id, sf::Vector2f pos)
{
    addTile(-1, pos);
}

const std::string& Layer::getTextureName()
{
    return texture_name;
}
