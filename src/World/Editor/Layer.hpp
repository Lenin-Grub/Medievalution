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
    Layer(int tileSize, sf::Vector2i board_size);

    void init();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void addTile(const int& id, sf::Vector2f pos);
    void removeTile(const int& id, sf::Vector2f pos);
    bool visible;
public:
    sf::Vector2i layer_size;
    const int    tile_size;
    int          tileset_cols;
    int          tileset_rows;
    int          board_tile_width;
    sf::Texture& tileset_texture;

    sf::VertexArray tile_map;
    std::vector<int> tile_ids;
private:
};