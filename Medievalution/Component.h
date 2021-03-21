#pragma once

class Entity;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	// Системный функционал! Это используется только в классе Entity!
	void setEntity(Entity* entity);							// поместить сущность
	Entity* getEntity() const;								// получить сущность

	virtual void initComponent() {};						// инициализация компонента
	virtual void deleteComponent() {};						// удаление компонента

	bool isDestroyedComponent() const;						// проверка уничтожен ли компонент?
	void destroyComponent();								// разрушаем компонент

	virtual void updateComponent() {};						// обновление компонента
	virtual void renderComponent() {};						// отрисовка компонента

private:
	Entity* _entity = nullptr;								// сущностиь 0
	bool _isDestroyed = false;								// по-умолчание не разрушаем компонент
};

