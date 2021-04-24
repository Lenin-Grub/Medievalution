#include "stdafx.h"
#include "Component.h"

void Component::setParent(Entity* entity)
{
	this->m_entity = entity;
}

Entity* Component::getParent() const
{
	return m_entity;
}

void Component::registerECS(ECS* ecs)
{
	this->m_ecs = ecs;
}

void Component::destroyComponent()
{
	isDestroyed = true;
}

bool Component::isComponentDestroyed() const
{
	return isDestroyed;
}
