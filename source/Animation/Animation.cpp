#include "Animation.hpp"

void Animator::addFrame(sf::IntRect rect)
{
	frames.push_back(rect);
}

void Animator::setFrameTime(float time)
{
	frameTime = time;
}

void Animator::update(float deltaTime)
{
	currentTime += deltaTime;
	if (!played)
	{
		sprite.setTextureRect(frames.at(currentFrame));
	}
	else
	{
		if (currentTime >= frameTime)
		{
			currentFrame = (currentFrame + 1) % frames.size();
			sprite.setTextureRect(frames.at(currentFrame));
			currentTime = 0.0f;
		}
	}
}

void Animator::play()
{
	played = true;
}

void Animator::stop()
{
	played = false;
}
