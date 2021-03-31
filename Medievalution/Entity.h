#pragma once
// îò

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void addComponent();													// добавить компонент
	void getComponent();													// получить компонент
	void hasCompinent();													// есть ли компонент
	
	void initEntity();														// инициализация сущности
	void updateEntity();													// обновление сущности
	void renderEntity();													// отрисовка сущности

private:
};

