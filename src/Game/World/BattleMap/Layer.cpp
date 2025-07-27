#include "Layer.hpp"
#include <Common/Log.h>

Layer::Layer(int tileSize, sf::Vector2i board_size, sf::Texture& texture, int index)
    : tile_size(tileSize)
    , layer_size (board_size)
    , tileset_texture(texture)
    , tileset_cols(0)
    , tileset_rows(0)
    , visible(true)
    , layer_index(index)
{
    tile_ids.resize(layer_size.x * layer_size.y, static_cast<int>(TileId::Empty));
}

bool Layer::init()
{
    try
    {
        tile_map.setPrimitiveType(sf::Quads);
        tile_map.resize(layer_size.x * layer_size.y * 4);

        for (int y = 0; y < layer_size.y; ++y)
        {
            for (int x = 0; x < layer_size.x; ++x) 
            {
                updateVertex(sf::Vector2f(x, y));
            }
        }

        tileset_cols = std::round(tileset_texture.getSize().x / tile_size);
        tileset_rows = std::round(tileset_texture.getSize().y / tile_size);

        return true;
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Layer initialization failed: {0}", e.what());
        return false;
    }
}

void Layer::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    float offset_x = -1.0f * layer_index;
    float offset_y = -(tile_size / 2) * layer_index;

    states.transform.translate(offset_x, offset_y);
    states.texture = &tileset_texture;
    target.draw(tile_map, states);
}

void Layer::addTile(int id, sf::Vector2f pos)
{
    sf::Vector2i grid_pos = worldToGrid(pos);

    if (grid_pos.x >= 0 && grid_pos.y >= 0 &&
        grid_pos.x < layer_size.x && grid_pos.y < layer_size.y)
    {
        int index = grid_pos.y * layer_size.x + grid_pos.x;
        if (index >= 0 && index < static_cast<int>(tile_ids.size())) 
        {
            tile_ids[index] = id;
            updateVertices();
        }
    }
}

void Layer::removeTile(const int& id, sf::Vector2f pos) noexcept
{
    addTile(static_cast<int>(TileId::Empty), pos);
}

void Layer::updateVertices() noexcept
{
    for (int y = 0; y < layer_size.y; ++y)
    {
        for (int x = 0; x < layer_size.x; ++x)
        {
            updateVertex(sf::Vector2f(x, y));
        }
    }
}

void Layer::updateVertex(sf::Vector2f pos) noexcept
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= layer_size.x || pos.y >= layer_size.y)
        return;

    int index = pos.x + pos.y * layer_size.x;
    int tile_Id = tile_ids[index];

    float base_x = pos.x * (tile_size / 2) - pos.y * (tile_size / 2);
    float base_y = (pos.x + pos.y) * (tile_size / 4);

    sf::Vertex* quad = &tile_map[index * 4];

    quad[0].position = sf::Vector2f(base_x, base_y);
    quad[1].position = sf::Vector2f(base_x + tile_size, base_y);
    quad[2].position = sf::Vector2f(base_x + tile_size, base_y + tile_size);
    quad[3].position = sf::Vector2f(base_x, base_y + tile_size);

    if (tile_Id == static_cast<int>(TileId::Empty))
    {
        quad[0].texCoords = sf::Vector2f(0, 0);
        quad[1].texCoords = sf::Vector2f(0, 0);
        quad[2].texCoords = sf::Vector2f(0, 0);
        quad[3].texCoords = sf::Vector2f(0, 0);
    }
    else
    {
        int tu = tile_Id % tileset_cols;
        int tv = tile_Id / tileset_cols;

        quad[0].texCoords = sf::Vector2f( tu      * tile_size,  tv      * tile_size);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size,  tv      * tile_size);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size, (tv + 1) * tile_size);
        quad[3].texCoords = sf::Vector2f( tu      * tile_size, (tv + 1) * tile_size);
    }
}

int Layer::getTileId(sf::Vector2i pos) const noexcept
{
    sf::Vector2i get_pos = pos;

    if (get_pos.x < 0 || get_pos.y < 0 || get_pos.x >= layer_size.x || get_pos.y >= layer_size.y)
        return static_cast<int>(TileId::Empty);

    int index = get_pos.x + get_pos.y * layer_size.x;

    return tile_ids.at(index);
}

sf::Vector2i Layer::getTileCoordinates(const sf::Vector2f& mouse_pos) const noexcept
{
    float offset_x = tile_size / 2.0f;
    float offset_y = tile_size / 8.0f;

    float layer_offset_x = 1.0f * layer_index;
    float layer_offset_y = (tile_size / 2) * layer_index;

    float mx = mouse_pos.x - offset_x + layer_offset_x;
    float my = mouse_pos.y - offset_y + layer_offset_y;

    float x_f = ((my / (tile_size / 4.0f)) + (mx / (tile_size / 2))) / 2.0f;
    float y_f = ((my / (tile_size / 4.0f)) - (mx / (tile_size / 2))) / 2.0f;

    int tile_x = static_cast<int>(std::floor(x_f));
    int tile_y = static_cast<int>(std::floor(y_f));

    if (tile_x < 0 || tile_y < 0 || tile_x >= layer_size.x || tile_y >= layer_size.y)
    {
        return sf::Vector2i(-1, -1);
    }

    return sf::Vector2i(tile_x, tile_y);
}

sf::Vector2f Layer::gridToWorld(sf::Vector2i gridPos) const
{
    float world_x = (gridPos.x - gridPos.y) * (tile_size / 2.0f);
    float world_y = (gridPos.x + gridPos.y) * (tile_size / 4.0f);
    return { world_x, world_y };
}

sf::Vector2i Layer::worldToGrid(sf::Vector2f worldPos) const
{
    float map_x = (worldPos.x / (tile_size / 2.0f) + worldPos.y / (tile_size / 4.0f)) / 2.0f;
    float map_y = (worldPos.y / (tile_size / 4.0f) - worldPos.x / (tile_size / 2.0f)) / 2.0f;
    return { static_cast<int>(std::floor(map_x)), static_cast<int>(std::floor(map_y)) };
}