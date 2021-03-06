#pragma once
#include "Component.h"

class Component;
class SystemBase
{
public:
	using Components = std::vector<std::shared_ptr<Component>>;

	SystemBase() = default;
	virtual ~SystemBase() = default;

	void getSystem() const;																// ����������	 �������


	template <typename T>
	std::shared_ptr<T> addComponent();													// �������� ���������

	template <typename T>
	std::shared_ptr<T> getComponentID() const;											// ������� ��������� ID

	template <typename T>
	std::shared_ptr<T> getComponentType() const;										// ������� ��������� Type

	template <typename T>
	bool hasComponentID();																// ������ ���� �� ��������� �� ID

	template <typename T>
	bool hasComponentType();															// ������ ���� �� ��������� �� Type

	void initSystem() {};																// ������������� �������
	virtual void updateSystem();																// ����������	 �������

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
