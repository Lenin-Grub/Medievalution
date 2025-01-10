#include "stdafx.h"
#include "ResourceManager.hpp"

ResourceLoader& ResourceLoader::instance()
{
    static ResourceLoader loader;
    return loader;
}

sf::Texture& ResourceLoader::getTexture(const std::string& name)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;
    else
    {
        auto path = findResource(name);
        if (path.empty())
            throw std::runtime_error("Resource not found: " + name);

        sf::Texture texture;
        if (!texture.loadFromFile(path))
            throw std::runtime_error("Failed to load texture: " + path);

        return textures.emplace(name, std::move(texture)).first->second;
    }
}

sf::Image& ResourceLoader::getImage(const std::string& name)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = images.find(name);
    if (it != images.end())
        return it->second;
    else
    {
        auto path = findResource(name);
        if (path.empty())
            throw std::runtime_error("Resource not found: " + name);

        sf::Image image;
        if (!image.loadFromFile(path))
            throw std::runtime_error("Failed to load texture: " + path);

        return images.emplace(name, std::move(image)).first->second;
    }
}

sf::Font& ResourceLoader::getFont(const std::string& name)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = fonts.find(name);
    if (it != fonts.end()) 
        return it->second;

    else
    {
        auto path = findResource(name);
        if (path.empty()) 
            throw std::runtime_error("Resource not found: " + name);

        sf::Font font;
        if (!font.loadFromFile(path)) 
            throw std::runtime_error("Failed to load font: " + path);

        return fonts.emplace(name, std::move(font)).first->second;
    }
}

sf::SoundBuffer& ResourceLoader::getSoundBuffer(const std::string& name)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = soundBuffers.find(name);
    if (it != soundBuffers.end())
        return it->second;
    else 
    {
        auto path = findResource(name);
        if (path.empty())
            throw std::runtime_error("Resource not found: " + name);

        sf::SoundBuffer soundBuffer;
        if (!soundBuffer.loadFromFile(path)) 
            throw std::runtime_error("Failed to load sound buffer: " + path);

        return soundBuffers.emplace(name, std::move(soundBuffer)).first->second;
    }
}

std::string ResourceLoader::findResource(const std::string& name)
{
    static std::filesystem::path resource_folder;
    if (resource_folder.empty())
        resource_folder = std::filesystem::current_path() / "resources";

    for (const auto& file : std::filesystem::recursive_directory_iterator(resource_folder))
        if (file.path().filename() == name)
        {
            LOG_INFO("{}", file.path().string());
            return file.path().string();
        }
    return std::filesystem::path().string();
}