#pragma once

class ResourceManager
    : private sf::NonCopyable
{
public:
    static ResourceManager& getInstance();

    template <typename T>
    T load(const std::string& file);
    void get(const std::string& file) const {};

private:
    std::unordered_map<std::string, sf::Texture>     m_textures;
    std::unordered_map<std::string, sf::Font>        m_fonts;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
};

template<typename T>
inline T ResourceManager::load(const std::string& file)
{
    *T::loadFromFile(file);
    return T();
}
