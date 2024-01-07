#include "Animation.hpp"

void Animation::init()
{
	texture.loadFromFile("resources/SptitreAtlas/Archer/Shot_1.png");
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 128, 128, 128));
	sf::Vector2i spriteSize = sf::Vector2i(128, 128);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
}

void Animation::update()
{
	elapsedTime += core::clock.getElapsedTime();
	float timeAsSecond = elapsedTime.asSeconds();
	int animFrame = static_cast<int>((timeAsSecond / animationDuration) * static_cast<float>(frameNum)) % frameNum;
	sprite.setTextureRect(sf::IntRect(animFrame * spriteSize.x, 0, spriteSize.x, spriteSize.y));
}
