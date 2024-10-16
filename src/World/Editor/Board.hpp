#pragma once
#include "../../Common/Common.h"

/**
    @class Updates this boards vertex array to create a grid of empty tiles. The "grid lines"
    are created by drawing two squares in each cell one pixel different in size. The
    larger square is a unique color to create a grid line effect.
*/

class Board 
    : public sf::Drawable
    , public sf::Transformable
{
public:
    Board(int tileSize, sf::Vector2i board_size);

    void initBoard();

    sf::Vector2i board_size;
    const int tile_size;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::VertexArray background;
};