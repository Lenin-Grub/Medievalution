#pragma once
#include "Observer.hpp"

class Observable
{
public:
	Observable() {};
	virtual ~Observable() = default;

	virtual void addObserver(Observer& observer) = 0;
	virtual void removeObserver(Observer& observer) = 0;
	virtual void notifyObservers() = 0;

private:

};