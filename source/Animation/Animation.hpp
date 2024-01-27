#pragma once
#include "../stdafx.h"
#include "../Core/Core.h"

class Frame 
{
public:
    Frame(sf::Sprite& sprite);
    void addFrame(sf::IntRect rect);
    void setCurrentFrame(int frameIndex);

    sf::Sprite getSpr();
    sf::IntRect getCurrentFrame();

    std::vector<sf::IntRect> getFrames();

private:
    sf::Sprite& sprite;
    int currentFrame = 0;
    std::vector<sf::IntRect> frames;
};

class Animation 
{
public:
    Animation(sf::Sprite& sprite);
    void addAnimation(std::string name);
    Frame* getAnimation(std::string name);

    void play();
    void stop();
    void update(float deltaTime);
    bool isPlay();

    bool played = true;
    int currentFrame = 0;
    float currentTime = 0.0f;
    float frameTime = 0.2f;
    std::map<std::string, Frame*> animations;
    Frame* currentAnimation = nullptr;
private:
    sf::Sprite& sprite;
};

class Animator 
{
public:
	Animator(sf::Sprite& sprite);
	virtual ~Animator();

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