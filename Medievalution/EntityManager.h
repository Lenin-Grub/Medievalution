#pragma once
// îò

class EntityManager
{
public:
	EntityManager();
	virtual ~EntityManager();

	void addEntity();															// добавить сущность
	void findEntity();															// найти сущность
	void findEachEntties();														// найти каждую сущность
	void destroyEntity();														// уничтожить сущность

	void initEntity();															// инициализация сущности
	void updateEntity();														// обновление сущности
	void renderEntity();														// отрисовка сущности

private:
};

