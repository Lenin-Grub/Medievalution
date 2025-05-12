#pragma once
#include "Layer.hpp"

/**
 * @class Editor
 * @brief A class representing an editor for managing layers and tiles.
 *
 * The Editor class inherits from sf::Drawable and sf::Transformable, providing
 * functionality to manage layers and tiles, including adding, removing, and
 * switching between layers. It also handles drawing operations.
 */
class Editor
    : public sf::Drawable
    , public sf::Transformable
{
public:
    Editor();
    ~Editor() = default;

    /// @brief Initializes the editor.
    bool init();

    /// @brief Adds a tile to the current layer.
    /// @param id The ID of the tile to add.
    /// @param pos The position where the tile should be added.
    void addTile(const int& id, sf::Vector2f pos);

    void removeTile(sf::Vector2f pos);

    /// @brief Adds a new layer with the specified name.
    /// @param name The name of the new layer.
    void addLayer(const std::string& name);

    /// @brief Removes the current layer.
    void removeLayer();

    /// @brief Switches to the layer with the specified ID.
    /// @param id The ID of the layer to switch to.
    /// @return The ID of the new current layer.
    int switchTo(const int& id);

    /// @brief Gets the texture of the tileset.
    /// @return A reference to the tileset texture.
    sf::Texture& getTilesetTexture();

    /// @brief Gets the size of a tile.
    /// @return The size of a tile.
    const int getTileSize() const;

    /// @brief Gets the width of the tileset sheet.
    /// @return The width of the tileset sheet.
    const int getSheetWidth() const;

    /// @brief Gets the height of the tileset sheet.
    /// @return The height of the tileset sheet.
    const int getSheetHeight() const;

    /// @brief Gets the index of the current layer.
    /// @return The index of the current layer.
    unsigned int getCurrentLayer() const;

    /// @brief Sets the current layer by index.
    /// @param index The index of the layer to set as current.
    void setCurrentLayer(size_t index);

    /// @brief Gets a constant reference to the layers.
    /// @return A constant reference to the vector of layers.
    const std::vector<std::unique_ptr<Layer>>& getLayers() const;

    /// @brief Draws the editor's content to the target.
    /// @param target The render target to draw to.
    /// @param states The render states to use for drawing.
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void saveMap(const std::string& filePath) const;

    void loadMap(const std::string& filePath);

private:
    std::vector<std::unique_ptr<Layer>> layers; ///< Vector of layers managed by the editor.
    unsigned int current_layer;                 ///< Index of the current layer.
};
