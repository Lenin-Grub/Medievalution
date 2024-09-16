#pragma once
#include "../Core/Core.h"

class Battleground
    : public sf::Drawable
 //   , public sf::Transformable
{
public:
    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

    void update(unsigned int x, unsigned int y, int tileId);

    sf::Texture     m_tileset;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::VertexArray vertices;
    sf::Vector2u    tile_size;
    sf::Vector2i    map_size;
    int             tile_id;
};