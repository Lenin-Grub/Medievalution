#include "stdafx.h"
#include "Animation.hpp"

Animator::Animator(sf::Sprite& sprite)
    : sprite        (sprite)
    , played        (true)
    , current_frame (0)
    , frame_time    (0.0f)
    , current_time  (0.0f)
    , frame_size    {64,64}
{
}

bool Animator::init(const std::string& name)
{
   sprite.setTexture(ResourceLoader::instance().getTexture(name));
   return true;
}

void Animator::addFrame(sf::IntRect rect)
{
    frames.push_back(rect);
}

void Animator::removeFrame(const int& id)
{
    if (!frames.empty())
    {
        frames.erase(frames.begin() + id);
        if (current_frame > 0)
            current_frame--;
    }
    else 
       LOG_ERROR("You can't remove this frame. Frames are empty!");
}

void Animator::addAnimation(const std::string& name)
{
    if (frames.empty())
    {
        LOG_ERROR("You can`t add animation. Frames are empty");
        return;
    }
    animations.emplace(name, frames);
    LOG_INFO("Size animation {}", animations.size());
}

void Animator::removeAnimation(const std::string& name)
{
    if (animations.empty())
    {
        LOG_ERROR("You can`t remove animation: \"{}\" not founded", name);
        return;
    }
    animations.erase(name);
}

std::vector<sf::IntRect> Animator::getAnimation(const std::string& name) const
{
    //TODO
    if (!animations.empty())
        return animations.find(name)->second;
}

void Animator::setFrameTime(float time)
{
    frame_time = time;
}

void Animator::setCurrentFrame(int frame)
{
    if (!frames.empty()) 
        current_frame = frame;
}

void Animator::update(float deltaTime)
{
    if (frames.empty())
        return;

    current_time += deltaTime;
    if (!played)
        sprite.setTextureRect(frames.at(current_frame));
    else
    {
        if (current_time >= frame_time)
        {
            current_frame = (current_frame + 1) % frames.size();
            sprite.setTextureRect(frames.at(current_frame));
            current_time = 0.0f;
        }
    }
}

void Animator::play(bool play)
{
    played = play;
}

void Animator::pause()
{
    played = false;
}

const int Animator::getCurrentFrame() const
{
    if (!frames.empty() && current_frame < frames.size()) 
        return current_frame;
    else 
        return 0;
}

const int Animator::getNextFrame() const
{
    if (!frames.empty() && current_frame < frames.size()-1)
        return current_frame + 1;
}

int Animator::getPrevFrame() const
{
    if (!frames.empty() && current_frame <= 0)
        return 0;
       
    if (!frames.empty() && current_frame > 0)
        return current_frame - 1;
}

const int Animator::getLastFrame()
{
    if (!frames.empty())
        return current_frame = frames.size()-1;
}

const int Animator::getFirstFrame()
{
    if (!frames.empty() && current_frame < frames.size())
        return current_frame = 0;
}

sf::Vector2i Animator::getFrameSize() const
{
    return sf::Vector2i();
}

void Animator::setFrameSize(const sf::Vector2i size)
{
}

const float Animator::getCurrentTime() const
{
    return current_time;
}

const float Animator::getFrameTime() const
{
    return frame_time;
}

const bool Animator::isPlayed() const
{
    return played;
}

const std::vector<sf::IntRect>& Animator::getFrames() const
{
    return frames;
}

const sf::IntRect& Animator::getCurrentFrameRect() const
{
    if (frames.empty())
        return sf::IntRect(0, 0, 64, 64);

    return frames.at(getCurrentFrame());
}