#pragma once
#include "../stdafx.h"
#include "../Core/Core.h"

class Animator {
public:
	Animator(sf::Sprite& sprite) : sprite(sprite) {}

	void addFrame(sf::IntRect rect);
	void setFrameTime(float time);
	void update(float deltaTime);
	void play();
	void stop();

	float frameTime = 0.2f;
	bool played = true;
private:
	sf::Sprite& sprite;
	std::vector<sf::IntRect> frames;
	float currentTime = 0.0f;
	unsigned int currentFrame = 0;
};