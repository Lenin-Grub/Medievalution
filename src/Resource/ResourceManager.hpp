#pragma once

class ResourceLoader 
{
public:
    static ResourceLoader& instance();

    sf::Texture& getTexture(const std::string& name);

    sf::Image& getImage(const std::string& name);

    sf::Font& getFont(const std::string& name);

    sf::SoundBuffer& getSoundBuffer(const std::string& name);

private:
    ResourceLoader() = default;

    std::string findResource(const std::string& name);

    std::unordered_map<std::string, sf::Texture>     textures;
    std::unordered_map<std::string, sf::Image>       images;
    std::unordered_map<std::string, sf::Font>        fonts;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::mutex                                       mutex;
};