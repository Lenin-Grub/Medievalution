#pragma once
//#include "ECS.h"
#include "Entity.h"


class ECS;
class SystemBase;
class Entity;
class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	void setParent(Entity* entity);														// устанавливаем родителя
	Entity* getParent() const;															// возвращаем родителя

	virtual void registerECS(ECS* ecs);													// регестрирует компонент в ECS
	//virtual void getComponentID(int ID) const;											// возвращает ID компонента
	//virtual void getComponentType() const;												// возвращает тип компонента

	virtual void destroyComponent();													// разрушаем компонент
	bool isComponentDestroyed() const;													// разрушен ли компонент?

private:
	using ComponentId = std::size_t;													// ID компонента
	ComponentId ID;
	ECS* m_ecs;
	std::string tag = "default";														// тэг
	Entity* m_entity = nullptr;
	bool isDestroyed = false;
};