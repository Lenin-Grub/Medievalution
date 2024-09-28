#include "../../stdafx.h"
#include "board.hpp"

Board::Board(int tileSize, int width, int height)
    : tile_size(tileSize)
    , board_width(width)
    , board_height(height)
{
}

void Board::initBoard()
{
    // Create a single vertex array to represent the entire grid
    background.setPrimitiveType(sf::Quads);

    for (int x = 0; x < board_height; x++)
    {
        for (int y = 0; y < board_width; y++)
        {
            // Define the vertices of the current cell
            sf::Vector2f topLeft(y * tile_size, x * tile_size);
            sf::Vector2f topRight((y + 1) * tile_size, x * tile_size);
            sf::Vector2f bottomRight((y + 1) * tile_size, (x + 1) * tile_size);
            sf::Vector2f bottomLeft(y * tile_size, (x + 1) * tile_size);

            // Add the vertices of cell with background outline color
            sf::Color grid_outline_color(18, 18, 18);
            background.append(sf::Vertex(topLeft, grid_outline_color));
            background.append(sf::Vertex(topRight, grid_outline_color));
            background.append(sf::Vertex(bottomRight, grid_outline_color));
            background.append(sf::Vertex(bottomLeft, grid_outline_color));

            // Add vertices of cell with the cell color
            sf::Color grid_cell_color(30, 48, 74);
            background.append(sf::Vertex(sf::Vector2f(topLeft.x + 1, topLeft.y + 1), grid_cell_color));
            background.append(sf::Vertex(sf::Vector2f(topRight.x - 1, topRight.y + 1), grid_cell_color));
            background.append(sf::Vertex(sf::Vector2f(bottomRight.x - 1, bottomRight.y - 1), grid_cell_color));
            background.append(sf::Vertex(sf::Vector2f(bottomLeft.x + 1, bottomLeft.y - 1), grid_cell_color));
        }
    }
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(background, states);
}