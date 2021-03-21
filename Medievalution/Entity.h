#pragma once
#include "Component.h"

class EntityManager;

class Entity
{
public:

	using Components = std::unordered_map<std::string, std::shared_ptr<Component>>;

	Entity() = default;
	~Entity() = default;

	// Работа с менеджером сущностей, в котором находится сущность.
	void setEntityManager(EntityManager* entityManager);						// поместить сущность менеджер
	EntityManager* getEntityManager() const;									// получить сущность менеджер

	template<typename T>
	std::shared_ptr<T> addComponent(const std::string& componentName);			// добавляем компонент

	template<typename T>
	std::shared_ptr<T> getComponent(const std::string& componentName) const;	// получить комонент по названию

	bool isDestroyedEntity() const;												// проверка разрушена ли сущность
	void destroyEntity();														// уничтожаем сущность

	void initEntity();															// инициализация
	void updateEntity();														// обновление
	void renderEntity();														// отрисовка

private:
	Components _components;
	EntityManager* _entityManager;
	bool _isDestroyed = false;													// по-умолчанию сущность не уничтожена
};

template<typename T>
inline std::shared_ptr<T> Entity::addComponent(const std::string& componentName)				// добавляем компонент
{
	auto component = _components.emplace(componentName, std::make_shared<T>()).first->second;	// создаем указанный компонент
	component->setEntity(this);																	// КАК ЭТА БОБУЙНЯ РАБОТАЕТ? ЭТА ХЕРЬ ДОЛЖНА БЫТЬ ИНИЦИИРОВАНА В Component.h
	return std::move(std::dynamic_pointer_cast<T>(component));									// ЧТО ОНО ВОЗВРАЩАЕТ
}

template<typename T>
inline std::shared_ptr<T> Entity::getComponent(const std::string& componentName) const			// получить компонент по названию
{
	auto it = _components.find(componentName);													// начинаем поиск

	if (it != _components.end())																// если не нашли
	{
		return std::dynamic_pointer_cast<T>(it->second);										// возвращаем ссылку на чет
	}

	return nullptr;																				// возвращаем кукишь с маслом
}
