#pragma once
#include "Layer.hpp"

class Editor
    :public sf::Drawable
    , public sf::Transformable
{
public:
    Editor();
    ~Editor() = default;

    void init();
    void addTile(const int& id, sf::Vector2f pos);
    void removeTile(const int& id, sf::Vector2f pos);

    void addLayer(const std::string& name);
    void removeLayer(const std::string& name);

    int switchTo(const int& id);

    sf::Texture& getTilesetTexture();

    const int getTileSize() const;
    const int getSheetWidth() const;
    const int getSheetHeight() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    int current_layer;
    std::vector<std::unique_ptr<Layer>> layers;
private:
};