#include "stdafx.h"
#include "TransformComponent.h"
#include "System.h"

void TransformComponent::moveGameObject(const sf::Vector2f& delta)
{
	sf::Transformable::move(delta.x * S::dtime, delta.y * S::dtime);
}
