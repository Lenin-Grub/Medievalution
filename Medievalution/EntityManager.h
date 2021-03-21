#pragma once
#include "Entity.h"

class Entity;

class EntityManager
{
public:

	using Entities = std::list<std::shared_ptr<Entity>>;// список сущеностей

	EntityManager() = default;
	~EntityManager() = default;

	std::shared_ptr<Entity> addEntity();				// добавить сущность

	void init();										// инициализация
	void update();										// обновление
	void render();										// рендер

private:
	Entities _entities;									// список сущностей
};

