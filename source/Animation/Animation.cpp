#include "Animation.hpp"

Animator::Animator(sf::Sprite& sprite)
	: sprite(sprite), played(true)
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

const int Animator::getCurrentFrame()
{
	return current_frame;
}

const float Animator::getCurrentTime()
{
	return current_time;
}

const float Animator::getFrameTime()
{
	return frame_time;
}

const bool Animator::isPlayed()
{
	return played;
}

const std::vector<sf::IntRect> Animator::getFrames()
{
	return frames;
}



Frame::Frame(sf::Sprite& sprite) : sprite(sprite) 
{
}

void Frame::addFrame(sf::IntRect rect) 
{
    frames.push_back(rect);
}

void Frame::setCurrentFrame(int frameIndex) 
{
    if (frameIndex >= 0 && frameIndex < frames.size()) 
	{
        currentFrame = frameIndex;
    }
}

sf::Sprite Frame::getSpr()
{
	return sprite;
}

sf::IntRect Frame::getCurrentFrame() 
{
    return frames[currentFrame];
}

std::vector<sf::IntRect> Frame::getFrames() 
{
    return frames;
}

Animation::Animation(sf::Sprite& sprite) : sprite(sprite) 
{
}

void Animation::addAnimation(std::string name) 
{
    animations[name] = new Frame(sprite);
}

Frame* Animation::getAnimation(std::string name) 
{
    if (animations.find(name) != animations.end()) 
	{
        return animations[name];
    }
    return nullptr;
}

void Animation::play() 
{
    played = true;
}

void Animation::stop() 
{
    played = false;
}

void Animation::update(float deltaTime) 
{
    if (played) 
	{
        currentTime += deltaTime;
		currentAnimation->getSpr().setTextureRect(currentAnimation->getFrames().at(currentFrame));

        if (currentTime >= frameTime) 
		{
            currentFrame = (++currentFrame) % currentAnimation->getFrames().size();
			currentAnimation->setCurrentFrame(currentFrame);
			currentAnimation->getSpr().setTextureRect(currentAnimation->getFrames().at(currentFrame));
            currentTime = 0.0f;
        }
    }
}

bool Animation::isPlay() 
{
    return played;
}