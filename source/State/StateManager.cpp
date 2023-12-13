#include "../stdafx.h"
#include "StateManager.h"


StateManager::StateManager():Observable()
{
}

void StateManager::addState(State* state)
{
	states.push(state);
	LOG_INFO("State added. Count of state {}", states.size());
}

void StateManager::endState()
{
	if (!states.empty())
	{
		states.top()->endState();
		LOG_INFO("State ended. Count of state {}", states.size());
	}
}

void StateManager::init()
{
	StateManager::getInstance().addState(new IntroState(&stateData));
}

void StateManager::changeState(State* state, bool replace = false)
{
	if (replace == true)
	{
		endState();
		StateManager::getInstance().states.pop();
		//TO DO добавить условие по короторому сперва завершится заменяемый стейт
		addState(state);
	}
	else
	{
		replace = false;
		addState(state);
	}
}

void StateManager::addObserver(Observer& observer)
{
	observers.push_back(&observer);
}

void StateManager::removeObserver(Observer& observer)
{
	observers.remove(&observer);
}

void StateManager::notifyObservers()
{
	std::list<Observer*>::iterator iterator = observers.begin();
	while (iterator != observers.end()) {
		(*iterator)->updateObserver();
		++iterator;
	}
}
