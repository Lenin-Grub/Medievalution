#pragma once
#include "IntroState.h"
#include "MainMenuState.h"
#include "SettingsState.h"
#include "GameState.h"

//------------------------------------------------------------------------------------------------------------------------
class StateManager
{
public:
	static StateManager& getInstance()
	{
		static StateManager single_instance;
		return single_instance;
	}
		
	template <class T>
	void addState(T state);
	void removeFirstState();						// ������� ������ �����
	void removeCurrentState();						// ������� ������� �����
	void removePrevState();							// ������� ���������� �����
	void endState();								// ����� �� ������
	void init();

//------------------------------------------------------------------------------------------------------------------------
public:
	std::list<State*> states;						// ���� ��������� ���� (������� ����, ����, ��������� � ��)
	StateData stateData;							// �������� � �������� ��������� ���������� � �������, �������� � ��

//------------------------------------------------------------------------------------------------------------------------
private:
	StateManager() {}
	StateManager(const StateManager& root) = delete;
	StateManager& operator=(const StateManager&) = delete;
};

//------------------------------------------------------------------------------------------------------------------------

template<class T>
inline void StateManager::addState(T state)
{
	states.push_back(state);
	LOG_INFO("State added. Count of state {}", states.size());
}
