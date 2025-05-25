#pragma once
#include "Animation/Animator/Animator.hpp"

class AnimationEditor
{
public:
    explicit AnimationEditor(sf::RenderWindow& window);

    void update(const float& delta_time);
    void gui();

private:
    void animationsPanel();
    void spriteSelector();
    void playbackButtons();
    void addFrameFromSelectedTile();
    void frameControls();
    void frameSelector();
    void spriteSheet();

private:
    Animator          animator;
    sf::RenderWindow& window;
    sf::Texture       texture;
    sf::Sprite        sprite;

    std::string       current_animation_name;
    float             animationSpeed = 1.0f;
    int               selected_frame_index = -1;
    int               selected_tile_index = -1;
    bool              is_playing = false;
    bool              show_context_menu = false;
    std::optional<int> context_menu_tile_index;
};