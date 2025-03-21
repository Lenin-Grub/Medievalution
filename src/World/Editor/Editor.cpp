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
        layers.push_back(std::make_unique<Layer>(32, sf::Vector2i(50, 50), ResourceLoader::instance().getTexture("Tileset1.png")));
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
   layers.push_back(std::make_unique<Layer>(32, sf::Vector2i(50, 50), ResourceLoader::instance().getTexture(name)));
   layers.back()->init();
   current_layer++;
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
        LOG_CRITICAL("Size of layers vector is empty!");
        return;
    }

    const auto& layers = getLayers();
    for (const auto& layer : layers)
    {
        if (layer->visible)
            target.draw(*layer);
    }
}
