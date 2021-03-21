#include "stdafx.h"
#include "EntityManager.h"

std::shared_ptr<Entity> EntityManager::addEntity()
{
	auto entity = std::make_shared<Entity>();
	entity->setEntityManager(this);
	_entities.push_back(entity);
	return std::move(entity);
}

void EntityManager::init()
{
	for (const auto& entity : _entities)					// пробегаемся по сущностями
	{
		entity->initEntity();								// инициализируем сущности
	}
}

void EntityManager::update()
{
	for (auto it = _entities.begin(); it != _entities.end();)	// пробегаемся по списку сущностей
	{
		std::shared_ptr<Entity>& entity = *it;					// хз че тут, не до конца понял, прбегаемся по итератору?

		if (entity->isDestroyedEntity())						// если сущность разрушена
		{
			it = _entities.erase(it);							// освобождаем место
		}
		else
		{
			entity->updateEntity();								// иначе обновляем сущность
			++it;												// бежим дальше по списку
		}
	}
}

void EntityManager::render()
{
		for (const auto& entity : _entities)
	{
		entity->renderEntity();									// отрисовываем сущности
	}
}
