#pragma once
#include "../../Common/Common.h"


enum class TileId
{
    Empty = -1, ///< Represents an empty or unoccupied tile.
};

class Layer 
    : public sf::Drawable
    , public sf::Transformable
{
public:
    Layer(int tileSize, sf::Vector2i board_size, sf::Texture& texture, int index);
    virtual ~Layer() = default;


    /// @brief Initializes the internal structure of the layer.
    /// @return True if initialization was successful, false otherwise.
    bool init();

    /// @brief Draws the layer to a render target.
    /// @param target Render target to draw to.
    /// @param states Current render states.
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    /// @brief Adds a tile at the given screen position.
    /// @param id ID of the tile in the tileset.
    /// @param pos Screen coordinates where the tile should be placed.
    void addTile(const int& id, sf::Vector2f pos) noexcept;

    /// @brief Removes a tile at the given screen position.
    /// @param id Unused parameter(always sets tile to Empty).
    /// @param pos Screen coordinates where the tile should be removed.
    void removeTile(const int& id, sf::Vector2f pos) noexcept;

    /// @brief Updates all tile vertices in the layer.
    void updateVertices() noexcept;

    /// @brief Updates a single tile vertex at the specified screen position.
    /// @param pos Screen coordinates of the tile to update.
    void updateVertex(sf::Vector2f pos) noexcept;

    /// @brief Gets the tile ID at the specified grid position.
    /// @param pos Grid coordinates(x, y) of the tile.
    /// @return The ID of the tile at the given position.
    int getTileId(sf::Vector2i pos) const noexcept;

    /// @brief Converts screen coordinates into tile grid coordinates.
    /// @param mouse_pos Screen position(e.g., mouse cursor).
    /// @return Grid coordinates(x, y) of the corresponding tile.
    sf::Vector2i getTileCoordinates(const sf::Vector2f& mouse_pos) const noexcept;

public:
    std::vector<int> tile_ids;        ///< List of tile IDs for each cell in row-major order.
    sf::VertexArray  tile_map;        ///< Vertex array representing the entire layer's geometry.
    sf::Vector2i     layer_size;      ///< Number of tiles in X and Y directions.
    sf::Texture&     tileset_texture; ///< Reference to the tileset texture.
    const int        tile_size;       ///< Width and height of a single tile in pixels.
    int              tileset_cols;    ///< Number of columns in the tileset texture.
    int              tileset_rows;    ///< Number of rows in the tileset texture.
    int              layer_index;     ///< Index of the layer (used for drawing offset).
    bool             visible;         ///< Visibility flag for toggling layer display.
    std::string      layer_name;      ///< Name of the layer for UI purposes.
};