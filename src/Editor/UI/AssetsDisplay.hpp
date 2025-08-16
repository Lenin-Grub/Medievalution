#pragma once
#include <UI/IDisplay.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <Resource/ResourceManager.hpp>

class AssetsDisplay : public UI::IDisplay
{
public:
    AssetsDisplay();
    ~AssetsDisplay();

    void draw() override;

private:
    void loadAssets();
    void displayAssets();
    void displayTextures();
    void displayMusics();
    void displaySounds();
    void displayFiles();
    void displayFonts();

    std::vector<std::filesystem::path> textures;
    std::vector<std::filesystem::path> musics;
    std::vector<std::filesystem::path> sounds;
    std::vector<std::filesystem::path> files;
    std::vector<std::filesystem::path> fonts;
};