#include "stdafx.h"
#include "Animation.hpp"

Animator::Animator(sf::Sprite& sprite)
    : sprite        (sprite)
    , played        (true)
    , current_frame (0)
    , frame_time    (0.0f)
    , current_time  (0.0f)
    //,frames{ sf::IntRect(0,0,0,0)}
{
}

void Animator::init(const std::string& name)
{
    sprite.setTexture(ResourceLoader::instance().getTexture(name));
}

void Animator::addFrame(sf::IntRect rect)
{
    frames.push_back(rect);
}

void Animator::removeFrmae(const int& id)
{
    if (!frames.empty())
    {
        frames.erase(frames.begin() + id);
        if (current_frame > 0)
        {
            current_frame--;
        }
         //   current_frame = 0;
    }
    else 
       LOG_ERROR("You can't remove frame. Frames are empty!");
}

void Animator::setFrameTime(float time)
{
    frame_time = time;
}

void Animator::setCurrentFrame(int frame)
{
    if (!frames.empty()) 
        current_frame = frame;
    else 
        LOG_ERROR("Frames are empty! Cannot set current frame.");
}

void Animator::update(float deltaTime)
{
    if (frames.empty())
    {
        //LOG_ERROR("Frames are empty! Cannot update animator.");
        return;
    }

    current_time += deltaTime;
    if (!played)
    {
        sprite.setTextureRect(frames.at(current_frame));
    }
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
    if (!frames.empty()) 
        return current_frame;
    else 
    {
        LOG_ERROR("Frames are empty! Cannot get current frame.");
        return 0;
    }
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