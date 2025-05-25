#pragma once
#include <SFML/Audio.hpp>
#include <Common/Common.h>
#include <Common/Log.h>
#include <unordered_map>
#include <mutex>
#include <string>

/// @brief Singleton class for loading and managing resources such as textures, images, fonts, and sound buffers.
class ResourceLoader
{
public:
    /// @brief Returns the singleton instance of the ResourceLoader.
    /// @return Reference to the singleton instance.
    static ResourceLoader& instance();

    /// @brief Gets a texture by name. If the texture is not found, a checkerboard texture is generated.
    /// @param name The name of the texture to retrieve.
    /// @return Reference to the requested texture.
    sf::Texture& getTexture(const std::string& name);

    /// @brief Gets an image by name.
    /// @param name The name of the image to retrieve.
    /// @return Reference to the requested image.
    sf::Image& getImage(const std::string& name);

    /// @brief Gets a font by name.
    /// @param name The name of the font to retrieve.
    /// @return Reference to the requested font.
    sf::Font& getFont(const std::string& name);

    /// @brief Gets a sound buffer by name.
    /// @param name The name of the sound buffer to retrieve.
    /// @return Reference to the requested sound buffer.
    sf::SoundBuffer& getSoundBuffer(const std::string& name);

private:
    /// @brief Private constructor to prevent instantiation.
    ResourceLoader() = default;

    /// @brief Finds the resource path based on the name.
    /// @param name The name of the resource to find.
    /// @return The path to the resource, or an empty string if not found.
    std::string findResource(const std::string& name);

    /// @brief Generates a checkerboard texture.
    /// @return The generated checkerboard texture.
    sf::Texture generateTexture();

    std::unordered_map<std::string, sf::Texture>     textures;     ///< Map of loaded textures.
    std::unordered_map<std::string, sf::Image>       images;       ///< Map of loaded images.
    std::unordered_map<std::string, sf::Font>        fonts;        ///< Map of loaded fonts.
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers; ///< Map of loaded sound buffers.
    std::mutex                                       mutex;        ///< Mutex for thread-safe access.
};
