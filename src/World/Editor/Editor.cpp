#include "../../stdafx.h"
#include "Editor.hpp"

Editor::Editor()
    :current_layer(0)
{
}

void Editor::addLayer()
{
    layers.push_back(std::make_unique<Board>(32,sf::Vector2i(50,50)));
}

void Editor::removeLayer(const int& id)
{
    if (id >= 0 && id < layers.size())
        layers.erase(layers.begin() + id);
}

int Editor::getLayer() const
{
    return current_layer;
}

void Editor::switchTo(const int& id)
{
    // ������������� ����� ������ ����� � ���� �����������������
}

void Editor::addTile(const int& id, sf::Vector2f pos)
{
    // �������� ����
}

void Editor::removeTile(const int& id, sf::Vector2f pos)
{
    // ������� ����
}

sf::Texture& Editor::getTilesetTexture()
{
    return tileset_texture;
}

void Editor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const auto& layer : layers)
    {
        states.transform *= getTransform();
        target.draw(*layer, states);
    }
}
