#include "../stdafx.h"
#include "Animation.hpp"

Animator::Animator(sf::Sprite& sprite)
	: sprite(sprite)
	, played(true)
	, frame_time (0.0f)
	, current_time (0.0f)
{
}

Animator::~Animator()
{
}

void Animator::addFrame(sf::IntRect rect)
{
	frames.push_back(rect);
}

void Animator::setFrameTime(float time)
{
	frame_time = time;
}

void Animator::setCurrentFrame(int frame)
{
	current_frame = frame;
}

void Animator::update(float deltaTime)
{
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
	return current_frame;
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

const std::vector<sf::IntRect> Animator::getFrames() const
{
	return frames;
}