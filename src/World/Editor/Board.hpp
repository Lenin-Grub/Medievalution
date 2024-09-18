#pragma once 
#include "../../Core/Core.h"

/**
    Updates this boards vertex array to create a grid of empty tiles. The "grid lines"
    are created by drawing two squares in each cell one pixel different in size. The
    larger square is a unique color to create a grid line effect.
*/

class Board : public sf::Drawable, public sf::Transformable
{
public:
    Board(int tileSize, int width, int height);

    void initBoard();

    const int tile_size;
    const int board_width;
    const int board_height;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    sf::VertexArray background;
};