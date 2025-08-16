#include "AssetsDisplay.hpp"
#include <imgui.h>
#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>

AssetsDisplay::AssetsDisplay()
{
    loadAssets();
}

AssetsDisplay::~AssetsDisplay()
{
}

void AssetsDisplay::draw()
{
    ImGui::Begin("Assets");
    displayAssets();
    ImGui::End();
}

void AssetsDisplay::loadAssets()
{   
    textures.clear();
    fonts.clear();
    musics.clear();
    sounds.clear();
    files.clear();

    try
    {
        const std::filesystem::path resources_path = "resources";
        if (!std::filesystem::exists(resources_path))
        {
            LOG_ERROR("Resources directory not found: {}", resources_path.string());
            return;
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(resources_path))
        {
            if (entry.is_regular_file())
            {
                const auto& path = entry.path();
                const auto extension = path.extension().string();
                
                std::string ext_lower = extension;
                std::transform(ext_lower.begin(), ext_lower.end(), ext_lower.begin(), ::tolower);

                if (ext_lower == ".png" || ext_lower == ".jpg" || ext_lower == ".jpeg")
                {
                    textures.push_back(path);
                }
                else if (ext_lower == ".ttf" || ext_lower == ".otf")
                {
                    fonts.push_back(path);
                }
                else if (ext_lower == ".wav")
                {
                    std::string full_path = path.string();
                    if (full_path.find("Musics") != std::string::npos)
                        musics.push_back(path);
                    else
                        sounds.push_back(path);
                }
                else
                {
                    files.push_back(path);
                }
            }
        }

        std::sort(textures.begin(), textures.end());
        std::sort(fonts.begin(), fonts.end());
        std::sort(musics.begin(), musics.end());
        std::sort(sounds.begin(), sounds.end());
        std::sort(files.begin(), files.end());
    }

    catch (const std::filesystem::filesystem_error& e)
    {
        LOG_CRITICAL("Filesystem error: %s", e.what());
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }

    catch (const std::exception& e)
    {
        LOG_CRITICAL("Exception: %s", e.what());
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void AssetsDisplay::displayAssets()
{
    const char* asset_types[] = { "Textures", "Fonts", "Musics", "Sounds", "Files" };
    static int current_asset_type = 0;

    ImGui::Text("Asset Type:");
    ImGui::SameLine();
    ImGui::Combo("##AssetType", &current_asset_type, asset_types, IM_ARRAYSIZE(asset_types));

    ImGui::Separator();

    switch (current_asset_type)
    {
        case 0: // Textures
            displayTextures();
            break;
        case 1: // Fonts
            displayFonts();
            break;
        case 2: // Musics
            displayMusics();
            break;
        case 3: // Sounds
            displaySounds();
            break;
        case 4: // Files
            displayFiles();
            break;
    }
}

void AssetsDisplay::displayTextures()
{
    if (textures.empty())
    {
        ImGui::Text("No textures found");
        return;
    }

    ImGui::Text("Texture Gallery (%d items):", static_cast<int>(textures.size()));
    ImGui::Separator();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

    const float image_size = 64.0f;
    const ImVec2 size(image_size, image_size);
    const ImVec2 uv0(0.0f, 0.0f);
    const ImVec2 uv1(1.0f, 1.0f);
    const ImVec4 background_color(0.0f, 0.0f, 0.0f, 1.0f);
    const ImVec4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);

    int columns = (int)(ImGui::GetContentRegionAvail().x / (image_size + 10));
    if (columns < 1) columns = 1;

    ImGui::Columns(columns, nullptr, false);

    for (const auto& texture_path : textures)
    {
        std::string texture_name = texture_path.filename().string();

        try
        {
            sf::Texture& tex = ResourceLoader::instance().getTexture(texture_name);
            ImTextureID texture_id = reinterpret_cast<ImTextureID>(tex.getNativeHandle());

            ImGui::PushID(texture_name.c_str());

            if (ImGui::ImageButton("##image", texture_id, size, uv0, uv1, background_color, tint_color))
            {
                std::cout << "Clicked on texture: " << texture_name << std::endl;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                
                ImGui::Text("%s", texture_name.c_str());
                
                ImVec2 tex_size(static_cast<float>(tex.getSize().x), static_cast<float>(tex.getSize().y));
                ImGui::Text("Size: %.0f x %.0f", tex_size.x, tex_size.y);
                
                ImGui::Separator();
                
                ImVec2 largeSize(256.0f, 256.0f);
                ImGui::Image(texture_id, largeSize, uv0, uv1, tint_color, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
                
                ImGui::EndTooltip();
            }

            ImGui::PopID();
            ImGui::NextColumn();
        }
        catch (const std::exception& ex)
        {
            ImGui::Button("ERROR", size);
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s", texture_name.c_str());
                ImGui::Text("Failed to load texture");
                ImGui::EndTooltip();
            }
            ImGui::NextColumn();
        }
    }

    ImGui::Columns(1);
    ImGui::PopStyleVar();
}

void AssetsDisplay::displayMusics()
{
    if (musics.empty())
    {
        ImGui::Text("No music files found");
        return;
    }

    ImGui::Text("Music Files (%d items):", static_cast<int>(musics.size()));
    ImGui::Separator();

    for (const auto& music : musics)
    {
        std::string music_name = music.filename().string();
        ImGui::Text("%s", music_name.c_str());
        
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("%s", music_name.c_str());
            ImGui::Text("Path: %s", music.string().c_str());
            ImGui::EndTooltip();
        }
    }
}

void AssetsDisplay::displaySounds()
{
    if (sounds.empty())
    {
        ImGui::Text("No sound files found");
        return;
    }

    ImGui::Text("Sound Files (%d items):", static_cast<int>(sounds.size()));
    ImGui::Separator();

    for (const auto& sound : sounds)
    {
        std::string sound_name = sound.filename().string();
        ImGui::Text("%s", sound_name.c_str());
        
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("%s", sound_name.c_str());
            ImGui::Text("Path: %s", sound.string().c_str());
            ImGui::EndTooltip();
        }
    }
}

void AssetsDisplay::displayFiles()
{
    if (files.empty())
    {
        ImGui::Text("No other files found");
        return;
    }

    ImGui::Text("Other Files (%d items):", static_cast<int>(files.size()));
    ImGui::Separator();

    for (const auto& file : files)
    {
        std::string file_name = file.filename().string();
        ImGui::Text("%s", file_name.c_str());
        
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("%s", file_name.c_str());
            ImGui::Text("Path: %s", file.string().c_str());
            ImGui::EndTooltip();
        }
    }
}

void AssetsDisplay::displayFonts()
{
    if (fonts.empty())
    {
        ImGui::Text("No font files found");
        return;
    }

    ImGui::Text("Font Files (%d items):", static_cast<int>(fonts.size()));
    ImGui::Separator();

    for (const auto& font : fonts)
    {
        std::string font_name = font.filename().string();
        ImGui::Text("%s", font_name.c_str());
        
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text("%s", font_name.c_str());
            ImGui::Text("Path: %s", font.string().c_str());            
            ImGui::EndTooltip();
        }
    }
}