#include "../stdafx.h"
#include "StateManager.h"

//------------------------------------------------------------------------------------------------------------------------

void StateManager::removeFirstState()
{
	if (states.size() > 1)
	{
		delete states.front();
		states.pop_front();
		LOG_INFO("State removed. Count of state {}", states.size());
	}
	else
	{
		LOG_ERROR("You cant delete first state, because you have only 1 state in list");
	}
}

//------------------------------------------------------------------------------------------------------------------------

void StateManager::removeCurrentState()
{
	if (states.size() > 1)
	{
		delete states.back();												// очичаем от стейта память
		states.pop_back();													// предпоследний стейт становится активным
		LOG_INFO("State removed. Count of state {}", states.size());
	}
	else
	{
		LOG_ERROR("You cant delete current state, because you have only 1 state in list");
	}
}

//------------------------------------------------------------------------------------------------------------------------

void StateManager::removePrevState()
{
	if (states.size() > 1)
	{
		auto i = std::prev(states.end());
		states.erase(--i);
		LOG_INFO("State removed. Count of state {}", states.size());
	}
	else
	{
		LOG_ERROR("You cant delete previouse state, because you have only 1 state in list");
	}
}

//------------------------------------------------------------------------------------------------------------------------

void StateManager::endState()
{
	if (!states.empty())
	{
		states.back()->endState();                                     // завершаем активный стейт в стеке
		LOG_INFO("State ended. Count of state {}", states.size());
	}
}

void StateManager::init()
{
	StateManager::getInstance().addState(new MainMenuState(&StateManager::getInstance().stateData));
	// если я удаляю первый стейт и остается только 1 стейт, я должен сделать его первым
}
