#pragma once 
#include "../../Core/Core.h"

class Board : public sf::Drawable, public sf::Transformable
{
public:
    //Board() : m_tile_size(0), m_board_width(0), m_board_height(0) {}
    Board(int tileSize, int width, int height);

    /*
        Updates this boards vertex array to create a grid of empty tiles. The "grid lines"
        are created by drawing two squares in each cell one pixel different in size. The
        larger square is a unique color to create a grid line effect.
    */
    void initBoard();

    const int m_tile_size;
    const int m_board_width;
    const int m_board_height;

private:
    sf::VertexArray m_background;

    // mark as override TODO
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};