#pragma once
#include "Component.h"
class TransformComponent :
    public Component,
    public sf::Transformable

{
public:
	TransformComponent() = default;
	~TransformComponent() = default;

	void moveGameObject(const sf::Vector2f & delta);
};

