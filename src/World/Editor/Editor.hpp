#pragma once
#include "board.hpp"

class Editor
    :public sf::Drawable
    , public sf::Transformable
{
public:
    Editor();
    ~Editor() = default;

    void addLayer();
    void removeLayer(const int& id);
    int  getLayer() const;
    void switchTo(const int& id);

    ///	@brie Updates the tile id at the given position. Xpos and Ypos are absolute coordinates
    /// that need to be converted into relative coordinates to find the clicked tile
    /// @param id The tile ID that you want place.
    /// @param pos The coordinates of the tile position.
    void addTile(const int& id, sf::Vector2f pos);

    void removeTile(const int& id, sf::Vector2f pos);

    /// @brief Returns a reference to the tileset texture.
    /// @return sf::Texture & The tileset texture.
    sf::Texture& getTilesetTexture();

    /// @brief Returns the size of each tile in the sprite sheet.
    /// @return int The tile size.
    const int getTileSize() const;

    /// @brief Returns the width of the sprite sheet.
    /// @return int The sheet width.
    const int getSheetWidth() const;

    /// @brief Returns the height of the sprite sheet.
    /// @return int The sheet height.
    const int getSheetHeight() const;

    int getTileCount() const;

    sf::Vector2u getTilePosition(int id) const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    std::vector< std::unique_ptr<Board>> layers;
    std::vector<int> tile_ids;
    std::string      file_name;

    sf::VertexArray  drawn_tiles;
    sf::Vector2i     board_size;
    sf::Texture      tileset_texture;
    
    int              current_layer;
    int              tile_size;
    int              tileset_cols;
    int              tileset_rows;
    int              board_tile_width;
};