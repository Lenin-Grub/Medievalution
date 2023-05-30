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
		delete states.back();												// ������� �� ������ ������
		states.pop_back();													// ������������� ����� ���������� ��������
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
		states.back()->endState();                                     // ��������� �������� ����� � �����
		LOG_INFO("State ended. Count of state {}", states.size());
	}
}

void StateManager::init()
{
	StateManager::getInstance().addState(new MainMenuState(&StateManager::getInstance().stateData));
	// ���� � ������ ������ ����� � �������� ������ 1 �����, � ������ ������� ��� ������
}
