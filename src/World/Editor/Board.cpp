#include "../../stdafx.h"
#include "board.hpp"

Board::Board(int tileSize, sf::Vector2i board_size)
    : tile_size(tileSize)
    , board_size (board_size)
    , grid_cell_color(30, 48, 74)
    , grid_outline_color(18, 18, 18)
{
}

void Board::initBoard()
{
    // Create a single vertex array to represent the entire grid
    tile_map.setPrimitiveType(sf::Quads);

    for (int x = 0; x < board_size.x; x++)
    {
        for (int y = 0; y < board_size.y; y++)
        {
            // Define the vertices of the current cell
            sf::Vector2f topLeft     ( y *      tile_size,  x *      tile_size);
            sf::Vector2f topRight    ((y + 1) * tile_size,  x *      tile_size);
            sf::Vector2f bottomRight ((y + 1) * tile_size, (x + 1) * tile_size);
            sf::Vector2f bottomLeft  ( y *      tile_size, (x + 1) * tile_size);

            // Add the vertices of cell with background outline color
            //sf::Color grid_outline_color(18, 18, 18);
            tile_map.append(sf::Vertex(topLeft,     grid_outline_color));
            tile_map.append(sf::Vertex(topRight,    grid_outline_color));
            tile_map.append(sf::Vertex(bottomRight, grid_outline_color));
            tile_map.append(sf::Vertex(bottomLeft,  grid_outline_color));

            // Add vertices of cell with the cell color
            //sf::Color grid_cell_color(30, 48, 74);
            tile_map.append(sf::Vertex(sf::Vector2f (topLeft.x     + 1, topLeft.y     + 1), grid_cell_color));
            tile_map.append(sf::Vertex(sf::Vector2f (topRight.x    - 1, topRight.y    + 1), grid_cell_color));
            tile_map.append(sf::Vertex(sf::Vector2f (bottomRight.x - 1, bottomRight.y - 1), grid_cell_color));
            tile_map.append(sf::Vertex(sf::Vector2f (bottomLeft.x  + 1, bottomLeft.y  - 1), grid_cell_color));
        }
    }
}

void Board::update()
{
    for (size_t i = 1; i < tile_map.getVertexCount(); i += 4)
    {
        // Ensure we don't go out of bounds
        if (i + 3 < tile_map.getVertexCount())
        {
            // Change the color of the cell vertices
            tile_map[i + 0].color = grid_cell_color; // Top-left
            tile_map[i + 1].color = grid_cell_color; // Top-right
            tile_map[i + 2].color = grid_cell_color; // Bottom-right
            tile_map[i + 3].color = grid_cell_color; // Bottom-left
        }
    }
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(tile_map, states);
}