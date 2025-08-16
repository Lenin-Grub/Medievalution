#include "BattleMap.hpp"

BattleMap::BattleMap()
    : current_layer_id(0)
    , selected_tile_id (0)
    , current_tile_id(0)
    , show_preview(false)
{
}

bool BattleMap::init()
{
    try
    {
        int layer_id = 0;
        layers.push_back(std::make_unique<Layer>(64, sf::Vector2i(50, 50), ResourceLoader::instance().getTexture("Tileset1.png"), layer_id));
        layers.back()->init();
        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}

void BattleMap::addTile(const int& id, sf::Vector2f pos) noexcept
{
    if (current_layer_id >= 0 && current_layer_id < layers.size())
        layers.at(current_layer_id)->addTile(id, pos);
}

void BattleMap::removeTile(sf::Vector2f pos) noexcept
{
    if (current_layer_id >= 0 && current_layer_id < layers.size())
        layers.at(current_layer_id)->addTile(static_cast<int>(TileId::Empty), pos);
}

void BattleMap::addLayer(const std::string& name) noexcept
{
   layers.push_back(std::make_unique<Layer>(64, sf::Vector2i(50, 50), ResourceLoader::instance().getTexture(name), ++current_layer_id));
   layers.back()->init();
}

void BattleMap::removeLayer() noexcept
{
    if (layers.size() > 1)
    {
        layers.erase(layers.begin() + current_layer_id);
        if (current_layer_id > 0)
            current_layer_id--;
    }
    else
    {
        LOG_ERROR("You can`t remove default layer! Layers: {0}", layers.size());
    }
}

int BattleMap::switchTo(const int& id) noexcept
{
    return current_layer_id = id;
}

sf::Texture& BattleMap::getTilesetTexture() noexcept
{
    return layers.at(current_layer_id)->tileset_texture;
}

const int BattleMap::getTileSize() const noexcept
{
    return layers.at(current_layer_id)->tile_size;
}

const int BattleMap::getSheetWidth() const noexcept
{
    return layers.at(current_layer_id)->tileset_cols;
}

const int BattleMap::getSheetHeight() const noexcept
{
    return layers.at(current_layer_id)->tileset_rows;
}

unsigned int BattleMap::getCurrentLayerID() const noexcept
{
    return current_layer_id;
}

void BattleMap::setCurrentLayerID(size_t index) noexcept
{
    if (index < layers.size())
        current_layer_id = index;
}

const std::vector<std::unique_ptr<Layer>>& BattleMap::getLayers() const noexcept
{
    return layers;
}

void BattleMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

    if (show_preview)
    {
        target.draw(tile_preview, states);
    }
}

void BattleMap::saveMap(const std::string& file_path) const
{
    nlohmann::json json_map;

    json_map["current_layer"] = current_layer_id;

    for (const auto& layer : layers)
    {
        nlohmann::json json_layer;

        json_layer["tile_size"] = layer->tile_size;
        json_layer["layer_size_x"] = layer->layer_size.x;
        json_layer["layer_size_y"] = layer->layer_size.y;
        json_layer["visible"] = layer->visible;
        json_layer["layer_index"] = layer->layer_index;
        json_layer["tileset_texture"] = "Tileset1.png"; // TODO: сделать динамическое имя
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

void BattleMap::loadMap(const std::string& file_path)
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

    current_layer_id = json_map.value("current_layer", 0);

    const int default_tile_size = 64;
    const int default_map_size  = 50;

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
        new_layer->updateVertices();

        layers.push_back(std::move(new_layer));
    }

    LOG_INFO("Map loaded successfully from {0}", file_path);
}

sf::Vector2i BattleMap::getMouseGridPosition() noexcept
{
    if (layers.empty())
        return { -1, -1 };

    const auto& layer = layers[current_layer_id];

    sf::Vector2f mousePos = common::mouse_pos_view;
    sf::Vector2i gridPos = layer->worldToGrid(mousePos);

    if (gridPos.x >= 0 && gridPos.y >= 0 && gridPos.x < layer->layer_size.x && gridPos.y < layer->layer_size.y)
        return gridPos;

    return { -1, -1 };
}

void BattleMap::setSelectedTile(int id) noexcept
{
    selected_tile_id = id;
}

void BattleMap::setShowPreview(bool show) noexcept
{
    show_preview = show;
}

bool BattleMap::isShowPreview() const noexcept
{
    return show_preview;
}

void BattleMap::setTileId(int id) noexcept
{
    current_tile_id = id;
}

int BattleMap::getTileId() const noexcept
{
    return current_tile_id;
}

void BattleMap::updatePreview(sf::Vector2f world_pos)
{
    if (layers.empty()) return;

    const auto& layer = layers[current_layer_id];
    sf::Vector2i gridPos = layer->worldToGrid(world_pos);
    preview_position = layer->gridToWorld(gridPos);

    tile_preview.setSize(sf::Vector2f(layer->tile_size, layer->tile_size));
    tile_preview.setPosition(preview_position);

    int cols = layer->tileset_cols;
    int x = (selected_tile_id % cols) * layer->tile_size;
    int y = (selected_tile_id / cols) * layer->tile_size;

    tile_preview.setTexture(&layer->tileset_texture);
    tile_preview.setTextureRect(sf::IntRect(x, y, layer->tile_size, layer->tile_size));
    tile_preview.setFillColor(sf::Color(255, 255, 255, 128));
}
