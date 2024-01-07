#pragma once
#include "../stdafx.h"
#include "../Core/Core.h"

class Animation
	:public sf::Drawable
{
public:
	Animation() {
		texture.loadFromFile("resources/SptitreAtlas/Archer/Shot_1.png");
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 128, 128, 128));
		sf::Vector2i spriteSize = sf::Vector2i(128, 128);
	};
	virtual ~Animation() {};

	void init();
	void update();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite sprite;
	float animationDuration = 0.001;
private:
	float timeAsSecond;
	sf::Time elapsedTime;

	sf::Vector2i spriteSize = sf::Vector2i(128, 128);
	int frameNum = 15;

	sf::Texture texture;
};