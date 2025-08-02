#include "AnimationDisplay.hpp"

AnimationDisplay::AnimationDisplay(sf::RenderWindow& window)
    :animation_editor(window)
{
}

void AnimationDisplay::draw()
{
    animation_editor.draw();
}

void AnimationDisplay::update(const float& delta_time)
{
    animation_editor.update(delta_time);
}
