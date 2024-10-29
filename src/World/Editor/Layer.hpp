#pragma once
#include "../../Common/Common.h"

/**
    @class Updates this boards vertex array to create a grid of empty tiles. The "grid lines"
    are created by drawing two squares in each cell one pixel different in size. The
    larger square is a unique color to create a grid line effect.
*/

class Layer 
    : public sf::Drawable
    , public sf::Transformable
{
public:
    Layer(int tileSize, sf::Vector2i board_size, sf::Texture& texture);

    void init();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void addTile(const int& id, sf::Vector2f pos);
    void removeTile(const int& id, sf::Vector2f pos);

public:
    std::vector<int> tile_ids;
    sf::VertexArray  tile_map;
    sf::Vector2i     layer_size;
    sf::Texture&     tileset_texture;
    const int        tile_size;
    int              tileset_cols;
    int              tileset_rows;
    bool             visible;

};