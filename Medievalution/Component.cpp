#include "stdafx.h"
#include "Component.h"

void Component::setEntity(Entity* entity)
{
	_entity = entity;
}

Entity* Component::getEntity() const
{
	return _entity;
}

bool Component::isDestroyedComponent() const
{
	return _isDestroyed;
}

void Component::destroyComponent()
{
	_isDestroyed = true;
}
