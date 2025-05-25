#pragma once
#include "Animation/Animator/Animator.hpp"

#include <json.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <Common/IconText.h>
#include <Common/Common.h>

class AnimationEditor 
{
public:
    explicit AnimationEditor(sf::RenderWindow& window);

    void update(const float& delta_time);

    void gui();

private:

    void drawTopPanel();

    void drawSpriteSelector();

    void drawAnimationsPanel();

    void drawFrameList();

    void drawFrameControls();

    void drawPlaybackControls();

    void drawSpriteSheet();

    void removeFrame(const std::string& animationName, size_t index);

    void updateSelectedTileFromFrame();

private:

    void addFrameFromSelectedTile();

private:
    sf::RenderWindow& window;
    sf::Texture texture;
    sf::Sprite sprite;
    Animator animator;

    std::string current_animation_name;
    int selected_frame_index;
    int selected_tile_index;

    int tileSize = 64;

    bool is_playing = false;
    bool is_looping = false;
};