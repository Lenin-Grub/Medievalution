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
	void removeFirstState();						// удалить первый стейт
	void removeLastState();
	void removeCurrentState();						// удалить текущий стейт
	void removePrevState();							// удалить предыдущий стейт
	void removeNextState();
	void removeAllStates();

	void endState();								// выйти из стейта
	void changeState();								// from X to Y

	void getCurrentState();
	void getPrevState();
	void getNextState();
	void getFirstState();
	void getLastState();

	void init();
	void update();

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

template<class T>
inline void StateManager::addState(T state)
{
	states.push_back(state);
	LOG_INFO("State added. Count of state {}", states.size());
}
