#pragma once
#include "Component.h"
#include "SystemBase.h"
#include "Entity.h"

class Entity;
class Component;
class SystemBase;
class ECS
{
public:
	using Entities = std::list<std::shared_ptr<Entity>>;
	using Systems = std::list<std::shared_ptr<SystemBase>>;

	ECS() = default;
	virtual ~ECS() = default;

	std::shared_ptr<Entity> addEntity();
	std::shared_ptr<SystemBase> removeEntity();
	
	template <typename T>
	std::shared_ptr<T> addSystem();

	void initEntities();														// инициализация сущностей
	void updateEntities();														// обновление сущностей

	void initSystems();															// инициализация систем
	void updateSystems();														// обновление систем

private:
	Entities m_entites;
	Systems  m_systems;
};

template<typename T>
inline std::shared_ptr<T> ECS::addSystem()
{
	auto ec_system = std::make_shared<T>();
	m_systems.push_back(ec_system);
	return std::move(ec_system);
}
