#pragma once
#include "Component.h"

class Component;
class SystemBase
{
public:
	using Components = std::vector<std::shared_ptr<Component>>;

	SystemBase() = default;
	virtual ~SystemBase() = default;

	void getSystem() const;																// возвращает	 систему


	template <typename T>
	std::shared_ptr<T> addComponent();													// добавить компонент

	template <typename T>
	std::shared_ptr<T> getComponentID() const;											// вернуть компонент ID

	template <typename T>
	std::shared_ptr<T> getComponentType() const;										// вернуть компонент Type

	template <typename T>
	bool hasComponentID();																// узнать есть ли компонент по ID

	template <typename T>
	bool hasComponentType();															// узнать есть ли компонент по Type

	void initSystem() {};																// инициализация системы
	virtual void updateSystem();																// обновление	 системы

private:
	Components m_components;
};

template<typename T>
inline std::shared_ptr<T> SystemBase::addComponent()
{
	std::cout << "component added to System" << std::endl;
	auto component = m_components.emplace(std::make_shared<T>()).first->second;
	component->setParent(this);
	return std::dynamic_pointer_cast<T>(component);
}
