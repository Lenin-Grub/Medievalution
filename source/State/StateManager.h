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

	void addState(State* state);
	void removeFirstState();						// удалить первый стейт
	void removeCurrentState();						// удалить текущий стейт
	void removePrevState();							// удалить предыдущий стейт
	void endState();								// выйти из стейта
	void init();

//------------------------------------------------------------------------------------------------------------------------
public:
	std::list<State*> states;						// стэк состояния игры (главное меню, игра, настройки и тд)
	StateData stateData;							// передает в дочерние состояния информацию о времени, событиях и тд

//------------------------------------------------------------------------------------------------------------------------
private:
	StateManager() {}
	StateManager(const StateManager& root) = delete;
	StateManager& operator=(const StateManager&) = delete;
};

//------------------------------------------------------------------------------------------------------------------------