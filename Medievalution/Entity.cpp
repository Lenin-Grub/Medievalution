#include "stdafx.h"
#include "Entity.h"

void Entity::setEntityManager(ECS* ecs)
{
	this->m_ecs = ecs;
}

ECS* Entity::getEntityManager() const
{
	return m_ecs;
}

void Entity::initEntity()
{
	//for (const auto& component : m_components )
	//{
	//	component.second;
	//}
}
