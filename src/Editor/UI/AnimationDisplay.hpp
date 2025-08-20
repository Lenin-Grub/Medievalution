#pragma once

#include <json.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <Common/IconText.h>
#include <Common/Common.h>
#include <UI/IDisplay.hpp>

#include <Animation/Animator/Animator.hpp>

class AnimationDisplay 
    : public UI::IDisplay
{
public:
    explicit AnimationDisplay(sf::RenderWindow& window);

    void update(const float& delta_time) override;

    void draw() override;

private:

    void drawTopPanel();

    void loadUnitAnimations(const std::string& unit_name);

    void drawSpriteSelector();

    void drawAnimationsPanel();

    void drawFrameList();

    void drawFrameControls();

    void drawPlaybackControls();

    void drawSpriteSheet();

    void removeFrame(const std::string& animationName, size_t index);

    void updateSelectedTileFromFrame();

    void applyDurationToAllFrames();

private:

    void addFrameFromSelectedTile();

private:
    sf::RenderWindow& window;
    sf::Texture       texture;
    sf::Sprite        sprite;
    Animator          animator;

    std::string current_animation_name;
    int         selected_frame_index;
    int         selected_tile_index;

    sf::Vector2i frame_size;

    bool is_playing = false;
    bool is_looping = false;
    bool is_flipp   = false;

private:
    std::vector<std::string> available_units;
    std::string current_unit = "Swordman";
    std::string current_loaded_texture = "";
    std::string current_texture_for_animation;
};