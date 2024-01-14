#pragma once
#include "../stdafx.h"
#include "../Core/Core.h"

class Animator {
public:
	Animator(sf::Sprite& sprite) : sprite(sprite),played(true) {}

	void addFrame(sf::IntRect rect);
	void setFrameTime(float time);
	void update(float deltaTime);
	void play();
	void stop();

	std::vector<sf::IntRect> frames;
	int currentFrame = 0;
	bool played = true;
	float currentTime = 0.0f;
	float frameTime = 0.2f;

private:
	sf::Sprite& sprite;
};