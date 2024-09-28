#pragma once
#include "../Core/Core.h"
#include "../Resource/ResourceManager.hpp"
#include "Province.h"

    /**
    @class WorldMap
    @brief A class representing a world map with provinces and their corresponding data.
    This class inherits from sf::Drawable and provides methods for initializing, drawing, and interacting with the world map. 
    */

class WorldMap 
    :public sf::Drawable
{
public:
    WorldMap ();
    virtual ~WorldMap ();

    /// @fn bool init()
    /// @brief Initializes the world map and its provinces.
    bool init();

    /// @brief Draws the world map on the target render target with the specified render states.
    /// @param target The render target to draw on.
    /// @param states The render states to use.
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /// @brief Gets the ID of the current province.
    /// @return The ID of the current province. */
    const int getProvinceID () const;

    /// @brief Gets the name of the current province.
    /// @return The name of the current province.
    const std::string getProvinceName() const;

    /// @brief Finds the ID of a province by its color.
    /// @param color The color of the province to find.
    /// @return The ID of the province, or -1 if not found.
    int findProvinceID (sf::Color color);

    /// @brief Gets the color of the current province.
    /// @return The color of the current province.
    sf::Color getColor ();

    /// @brief Finds the center of a province by its color.
    /// @param provinceColor The color of the province to find.
    /// @return The center of the province, or (-1, -1) if not found.
    sf::Vector2f findProvinceCenter (sf::Color provinceColor) const;

    /// @brief Gets the center of a province by its name.
    /// @param provinceName The name of the province to find.
    /// @return The center of the province, or (0, 0) if not found.
    sf::Vector2f getProvinceCenter  (const std::string& provinceName) const;

    /// @brief Gets the center of a province by its color.
    /// @param color The color of the province to find.
    /// @return The center of the province, or (0, 0) if not found.
    sf::Vector2f getProvinceCenter  (const sf::Color& color) const;

    /// @brief Initializes the province data.
    /// @return True if initialization is successful, false otherwise.
    bool         initProvinceData   ();

public:
    int                   load_progress; ///< @brief The load progress of the world map. 
    float                 transparency;  ///< @brief The transparency of the world map.
    sf::Shader            shader;        ///< @brief The shader used for rendering the world map.
    sf::Color             select_color;  ///< @brief The color used for selecting provinces.
    sf::Image             map_image;     ///< @brief The image of the world map. 
    std::vector<Province> provinces;     ///< @brief A vector of provinces on the world map.
    sf::CircleShape       shape;         ///< @brief The shape of the world map.

private:

    /// @brief Loads the provinces map.
    /// @private
    void loadProvincesMap();

    /// @brief Loads the map data.
    /// @private
    void loadMapData     ();

    /// @brief Sets the uniforms for the shader.
    /// @private
    void setUniforms     ();

    /// @brief Loads the shader.
    /// @private
    void loadShader      ();

    /// @brief Checks if the mouse is on the map.
    /// @return True if the mouse is on the map, false otherwise.
    /// @private
    bool isMouseOnMap    () const;

private:
    Province      province;
    sf::Color     current_color;
    sf::Texture   map_texture;
    sf::Texture   s_texture;
    sf::Sprite    s_province_map;
    sf::Sprite    s_texture_map;

    std::ifstream file;
    std::string   id, r, g, b, name, comment;

    float         height;
    float         width;
};