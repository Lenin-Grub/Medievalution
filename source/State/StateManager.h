#pragma once
#include "IntroState.h"
#include "MainMenuState.h"
#include "SettingsState.h"
#include "GameState.h"
#include "../Observer/Observer.hpp"

//------------------------------------------------------------------------------------------------------------------------
class StateManager
	: public Observer
{
public:
	static StateManager& getInstance()
	{
		static StateManager single_instance;
		return single_instance;
	}
//------------------------------------------------------------------------------------------------------------------------
	void addState(State* state);
	void removeFirstState();
	void removeCurrentState();
	void removePrevState();	
	void endState();		
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
	void updateObserver() override;
};
//------------------------------------------------------------------------------------------------------------------------