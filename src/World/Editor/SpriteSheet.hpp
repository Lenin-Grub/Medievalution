#pragma once
#include "../../Core/Core.h"
#include "Board.hpp"

class SpriteSheet : public sf::Drawable, public sf::Transformable
{
public:
    SpriteSheet() : m_tile_size(0), m_tileset_cols(0), m_tileset_rows(0) {}
    SpriteSheet(std::string file_name, int tile_size)
        : m_file_name(file_name), m_tile_size(tile_size) {}

    /*
        Loads a texture from a file, calculates the # of columns and rows, retrieves
        board dimensions, resizes a vector to store tile IDs
    */
    void import(Board & board);

    /*
        Iterates through the vertex array setting the position of each quad.
        Maps a texture to each quad based on the value of the tile id
    */
    void merge_tiles();

    /*
        Updates the tile id at the given position. Xpos and Ypos are absolute coordinates
        that need to be converted into relative coordinates to find the clicked tile
    */
    void add_tile_id(int id, int xpos, int ypos);

    /*
        Creates a sf::Image out of the vertex array and exports the file as an image to
        the given file name
    */
    void export_world(std::string file_name);

    // Getters to Access SpriteSheet info
    sf::Texture& get_tileset_texture()
    {
        return m_tilesetTexture;
    }
    int get_tile_size()
    {
        return m_tile_size;
    }
    int get_sheet_width()
    {
        return m_tileset_cols;
    }
    int get_sheet_height()
    {
        return m_tileset_rows;
    }

private:
    sf::Texture m_tilesetTexture; // Sprite sheet image texture
    std::string m_file_name;
    int m_tile_size; // width/height of single tile in pixels
    int m_tileset_cols;
    int m_tileset_rows;

    int m_boardWidth;
    int m_boardHeight;
    int m_board_tileWidth;

    std::vector<int> m_tile_ids;   // Texture Mapping IDs
    sf::VertexArray m_drawn_tiles; // Tiles that are drawn by user

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tilesetTexture;

        // draw the vertex array
        target.draw(m_drawn_tiles, states);
    }
};