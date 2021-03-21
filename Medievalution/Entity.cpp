#include "stdafx.h"
#include "Entity.h"

void Entity::setEntityManager(EntityManager* entityManager)
{
	_entityManager = entityManager;
}

EntityManager* Entity::getEntityManager() const
{
	return _entityManager;
}

bool Entity::isDestroyedEntity() const										// проверка уничтожена ли сущность
{
	return _isDestroyed;
}

void Entity::destroyEntity()
{
	_isDestroyed = true;
}

void Entity::initEntity()
{
	for (const auto& component : _components)
	{
		component.second->initComponent();								// инициализация компонента
	}
}

void Entity::updateEntity()
{
	for (auto it = _components.begin(); it != _components.end();)		// пробегаемся по списку компонентов
	{
		if (it->second->isDestroyedComponent())							// если компонента уничтожен
		{
			it->second->deleteComponent();								// удаляем компонент
			it = _components.erase(it);									// осовбождаем место
		}
		else
		{
			it->second->updateComponent();								// иначе обновляем компонент
			++it;														// переходим к следующему компоненту в списке
		}
	}
}

void Entity::renderEntity()
{
	for (const auto& component : _components)
	{
		component.second->renderComponent();							// отрисовываем
	}
}
