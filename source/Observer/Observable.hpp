#pragma once
#include "Observer.hpp"

class Observable
{
public:
	virtual ~Observable() = default;
	// добавить наблюдателя
	virtual void addObserver(Observer& observer) = 0;
	// удалить наблюдателя
	virtual void removeObserver(Observer& observer) = 0;
	// оповестить подписчиков
	virtual void notifyObservers() = 0;

private:
};