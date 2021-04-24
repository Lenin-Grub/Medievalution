#pragma once
#include "Component.h"
#include "ECS.h"

class Component;
class ECS;
class Entity
{
public:
	using Components = std::unordered_map<std::string, std::shared_ptr<Component>>;

	Entity() = default;
	virtual ~Entity() = default;

	void setEntityManager(ECS* ecs);
	ECS* getEntityManager() const;

	template <typename T>
	std::shared_ptr<T> addComponent(const std::string& id);								// добавить компонент

	template <typename T>
	std::shared_ptr<T> removeComponent();												// убрать компонент

	template <typename T>
	bool hasComponentID();																// узнать есть ли компонент по ID

	template <typename T>
	bool hasComponentType();															// узнать есть ли компонент по Type

	//virtual void setEntityTag();														// установить тэг сущности
	//virtual void getEntityTag() const;												// получить	  тэг сущности

	void getEntityID() const;															// возвращает ID  сущности
	void getEntityTag() const;															// возвращает тэг сущности

	void destroyEntity();																// уничтожить	  сущность
	bool isEntityDestroyed();															// уничтожена ли  сущность

	void initEntity();
	void updateEntity();

private:
	using EntityId = std::size_t;														// ID сущности
	EntityId entityID;
	Components m_components;
	ECS* m_ecs;
};

template<typename T>
inline std::shared_ptr<T> Entity::addComponent(const std::string& id)
{
	std::cout <<"component added to Entity"<< std::endl;
	auto component = m_components.emplace(id,std::make_shared<T>()).first->second;
	component->setParent(this);
	return std::move(std::dynamic_pointer_cast<T>(component));
}
