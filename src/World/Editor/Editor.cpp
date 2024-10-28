#include "../../stdafx.h"
#include "Editor.hpp"

Editor::Editor()
    :current_layer(0)
{
}

void Editor::init()
{
    layers.push_back(std::make_unique<Layer>(32, sf::Vector2i(50, 50)));
    layers.back()->init();
}


void Editor::addTile(const int& id, sf::Vector2f pos)
{
    if (current_layer >= 0 && current_layer < layers.size())
        layers.at(current_layer)->addTile(id, pos);
}

void Editor::removeTile(const int& id, sf::Vector2f pos)
{
    layers.at(current_layer)->removeTile(id, pos);
}

void Editor::addLayer(const std::string& name)
{
   layers.push_back(std::make_unique<Layer>(32, sf::Vector2i(50, 50)));
   layers.back()->init();
}

void Editor::removeLayer(const std::string& name)
{
    if (!layers.empty())
    {
        layers.pop_back();
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

void Editor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    size_t initialSize = layers.size();
    for (size_t i = 0; i < initialSize; ++i)
    {
        if (layers.size() != initialSize)
        {
            std::cerr << "Error: Size of layers vector changed during iteration!" << std::endl;
            break;
        }
        states.transform *= getTransform();
        target.draw(*layers.at(i), states);
    }

    if (layers.size() != initialSize)
        std::cerr << "Error: Size of layers vector changed during iteration!" << std::endl;
}
