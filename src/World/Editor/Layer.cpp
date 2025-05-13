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
                // Базовые координаты тайла
                float base_x = x * tile_size;
                float base_y = y * (tile_size / 2); // Уменьшаем шаг по вертикали

                // Смещение для четных рядов
                if (y % 2 == 0)
                {
                    base_x += tile_size / 2 ; // Смещение вправо
                }

                // Создаем вершины для тайла
                sf::Vertex topLeft(sf::Vector2f(base_x, base_y), sf::Vector2f(0, 0));
                sf::Vertex topRight(sf::Vector2f(base_x + tile_size, base_y), sf::Vector2f(tile_size, 0));
                sf::Vertex bottomRight(sf::Vector2f(base_x + tile_size, base_y + tile_size), sf::Vector2f(tile_size, tile_size));
                sf::Vertex bottomLeft(sf::Vector2f(base_x, base_y + tile_size), sf::Vector2f(0, tile_size));

                tile_map.append(topLeft);
                tile_map.append(topRight);
                tile_map.append(bottomRight);
                tile_map.append(bottomLeft);
            }
        }

        tileset_cols = std::round(tileset_texture.getSize().x / tile_size);
        tileset_rows = std::round(tileset_texture.getSize().y / tile_size);
        tile_ids.resize(static_cast<int64_t>(layer_size.x * layer_size.y), -1);

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

    // Преобразуем координаты мыши в индексы тайла
    sf::Vector2i tile_coords = getTileCoordinates(pos);
    int x = tile_coords.x;
    int y = tile_coords.y;

    if (x < 0 || y < 0 || x >= layer_size.x || y >= layer_size.y)
    {
        LOG_WARN("Invalid tile coordinates: ({0}, {1})", x, y);
        return;
    }

    int index = x + y * layer_size.x;
    tile_ids.at(index) = id;

    sf::Vertex* quad = &tile_map[static_cast<int64_t>(index * 4)];

    // Дополнительное вертикальное смещение
    int additional_offset = 16;

    // Базовые координаты тайла
    float base_x = x * tile_size;
    float base_y = y * (tile_size / 2) - y * additional_offset;

    // Смещение для четных рядов
    if (y % 2 == 0)
    {
        base_x += tile_size / 2; // Смещение вправо
    }

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

        quad[0].texCoords = sf::Vector2f(tu * tile_size, tv * tile_size);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size, tv * tile_size);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size, (tv + 1) * tile_size);
        quad[3].texCoords = sf::Vector2f(tu * tile_size, (tv + 1) * tile_size);
    }

    // Установка позиций вершин с учетом смещения
    quad[0].position = sf::Vector2f(base_x, base_y);
    quad[1].position = sf::Vector2f(base_x + tile_size, base_y);
    quad[2].position = sf::Vector2f(base_x + tile_size, base_y + tile_size);
    quad[3].position = sf::Vector2f(base_x, base_y + tile_size);
}

void Layer::removeTile(const int& id, sf::Vector2f pos)
{
    addTile(-1, pos);
}

const std::string& Layer::getTextureName()
{
    return texture_name;
}

sf::Vector2i Layer::getTileCoordinates(const sf::Vector2f& mouse_pos) const
{
    int additional_offset = 16;
    // Корректируем координаты мыши с учетом вертикального смещения
    float corrected_mouse_x = mouse_pos.x;
    float corrected_mouse_y = mouse_pos.y;

    // Вычисляем предполагаемый индекс строки
    int tile_y = static_cast<int>((corrected_mouse_y + (tile_size / 4)) / ((tile_size / 2) - additional_offset));

    // Корректируем горизонтальное смещение для четных и нечетных рядов
    if (tile_y % 2 == 0)
    {
        corrected_mouse_x -= tile_size / 2; // Смещаем влево для четных рядов
    }

    // Вычисляем индексы тайла
    int tile_x = static_cast<int>(corrected_mouse_x / tile_size);

    // Проверяем границы
    if (tile_x < 0 || tile_y < 0 || tile_x >= layer_size.x || tile_y >= layer_size.y)
    {
        return sf::Vector2i(-1, -1); // Возвращаем недействительные координаты
    }

    return sf::Vector2i(tile_x, tile_y);
}