#include "../../stdafx.h"
#include "Editor.hpp"

Editor::Editor()
    :current_layer(0)
{
}

bool Editor::init()
{
    try
    {
        layers.push_back(std::make_unique<Layer>(64, sf::Vector2i(50, 50), ResourceLoader::instance().getTexture("Tileset1.png"), 0));
        layers.back()->init();
        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}


void Editor::addTile(const int& id, sf::Vector2f pos)
{
    if (current_layer >= 0 && current_layer < layers.size())
        layers.at(current_layer)->addTile(id, pos);
}

void Editor::removeTile(sf::Vector2f pos)
{
    if (current_layer >= 0 && current_layer < layers.size())
        layers.at(current_layer)->addTile(0, pos);
}

void Editor::addLayer(const std::string& name)
{
   //current_layer++;
   layers.push_back(std::make_unique<Layer>(64, sf::Vector2i(50, 50), ResourceLoader::instance().getTexture(name), ++current_layer));
   layers.back()->init();
}

void Editor::removeLayer()
{
    if (layers.size() > 1)
    {
        layers.erase(layers.begin() + current_layer);
        if (current_layer > 0)
            current_layer--;
    }
    else
    {
        LOG_ERROR("You can`t remove default layer! Layers: {0}", layers.size());
    }
}

int Editor::switchTo(const int& id)
{
    return current_layer = id;
}

sf::Texture& Editor::getTilesetTexture()
{
    return layers.at(current_layer)->tileset_texture;
}

const int Editor::getTileSize() const
{
    return layers.at(current_layer)->tile_size;
}

const int Editor::getSheetWidth() const
{
    return layers.at(current_layer)->tileset_cols;
}

const int Editor::getSheetHeight() const
{
    return layers.at(current_layer)->tileset_rows;
}

unsigned int Editor::getCurrentLayer() const 
{
    return current_layer;
}

void Editor::setCurrentLayer(size_t index) 
{
    if (index < layers.size()) 
        current_layer = index;
}

const std::vector<std::unique_ptr<Layer>>& Editor::getLayers() const 
{
    return layers;
}

void Editor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (layers.empty())
    {
        LOG_CRITICAL("No layers to draw!");
        return;
    }

    for (const auto& layer : layers)
    {
        if (layer->visible)
            target.draw(*layer, states);
    }
}


void Editor::saveMap(const std::string& file_path) const
{
    nlohmann::json json_map;

    json_map["current_layer"] = current_layer;

    for (const auto& layer : layers)
    {
        nlohmann::json json_layer;

        json_layer["tile_size"] = layer->tile_size;
        json_layer["layer_size_x"] = layer->layer_size.x;
        json_layer["layer_size_y"] = layer->layer_size.y;
        json_layer["visible"] = layer->visible;
        json_layer["layer_index"] = layer->layer_index;
        json_layer["tileset_texture"] = "Tileset1.png"; // TODO: сделать динамическое им€
        json_layer["tile_ids"] = layer->tile_ids;

        json_map["layers"].push_back(json_layer);
    }

    std::ofstream file(file_path);
    if (!file.is_open())
    {
        LOG_ERROR("Failed to open file for saving: {0}", file_path);
        return;
    }

    file << json_map.dump(4);
    file.close();

    LOG_INFO("Map saved successfully to {0}", file_path);
}

void Editor::loadMap(const std::string& file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        LOG_ERROR("Failed to open file for loading: {0}", file_path);
        return;
    }

    nlohmann::json json_map;
    try
    {
        file >> json_map;
    }
    catch (const nlohmann::json::exception& e)
    {
        LOG_ERROR("JSON parsing error: {0}", e.what());
        return;
    }

    layers.clear();

    current_layer = json_map.value("current_layer", 0);

    const int default_tile_size = 64;
    const int default_map_size = 50;

    for (const auto& json_layer : json_map["layers"])
    {
        int tile_size = json_layer.value("tile_size", default_tile_size);
        sf::Vector2i layer_size(
            json_layer.value("layer_size_x", default_map_size),
            json_layer.value("layer_size_y", default_map_size)
        );

        bool visible = json_layer.value("visible", true);
        int layer_index = json_layer.value("layer_index", 0);

        std::string texturePath = json_layer.value("tileset_texture", "Tileset1.png");

        sf::Texture* texture = &ResourceLoader::instance().getTexture(texturePath);
        if (!texture || !texture->getSize().x || !texture->getSize().y)
        {
            LOG_ERROR("Failed to load texture: {0}", texturePath);
            continue;
        }

        auto new_layer = std::make_unique<Layer>(tile_size, layer_size, *texture, layer_index);
        new_layer->init();
        new_layer->visible = visible;

        const auto& tile_ids_array = json_layer["tile_ids"];
        new_layer->tile_ids = tile_ids_array.get<std::vector<int>>();

        // ¬осстанови вершины на основе tile_ids
        for (int y = 0; y < layer_size.y; ++y)
        {
            for (int x = 0; x < layer_size.x; ++x)
            {
                int index = x + y * layer_size.x;
                int tileId = new_layer->tile_ids[index];

                // »спользуем те же формулы, что и в init()
                float base_x = x * (tile_size / 2) - y * (tile_size / 2); // tile_size/2 == 32
                float base_y = (x + y) * 16.0f;

                sf::Vertex* quad = &new_layer->tile_map[index * 4];

                quad[0].position = sf::Vector2f(base_x, base_y);
                quad[1].position = sf::Vector2f(base_x + tile_size, base_y);
                quad[2].position = sf::Vector2f(base_x + tile_size, base_y + tile_size);
                quad[3].position = sf::Vector2f(base_x, base_y + tile_size);

                if (tileId == -1)
                {
                    quad[0].texCoords = sf::Vector2f(0, 0);
                    quad[1].texCoords = sf::Vector2f(0, 0);
                    quad[2].texCoords = sf::Vector2f(0, 0);
                    quad[3].texCoords = sf::Vector2f(0, 0);
                }
                else
                {
                    int tu = tileId % new_layer->tileset_cols;
                    int tv = tileId / new_layer->tileset_cols;

                    quad[0].texCoords = sf::Vector2f(tu * tile_size, tv * tile_size);
                    quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size, tv * tile_size);
                    quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size, (tv + 1) * tile_size);
                    quad[3].texCoords = sf::Vector2f(tu * tile_size, (tv + 1) * tile_size);
                }
            }
        }

        layers.push_back(std::move(new_layer));
    }

    LOG_INFO("Map loaded successfully from {0}", file_path);
}


sf::Vector2i Editor::getMouseGridPosition()
{
    const float tileW = 64, tileH = 32;
    const float halfW = tileW / 2, halfH = tileH / 2;

    float mx = common::mouse_pos_view.x; /*+ common::layer_index*/
    float my = common::mouse_pos_view.y; /*+ halfH * common::layer_index*/

    int tileX = (my / halfH + mx / halfW) / 2;
    int tileY = (my / halfH - mx / halfW) / 2;

    float screenX = (tileX - tileY) * halfW;
    float screenY = (tileX + tileY) * halfH;

    float relX = common::mouse_pos_view.x - screenX;
    float relY = common::mouse_pos_view.y - screenY;

    return common::mouse_pos_grid = { tileX, tileY };
}