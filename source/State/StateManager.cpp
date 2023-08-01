#include "../stdafx.h"
#include "StateManager.h"

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
 StateManager::getInstance().addState(new MainMenuState(&stateData));
}