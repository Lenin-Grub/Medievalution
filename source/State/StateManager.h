#pragma once
#include "IntroState.h"
#include "MainMenuState.h"
#include "SettingsState.h"
#include "GameState.h"
#include "../Observer/Observer.hpp"

class StateManager : public Observable
{
public:
	static StateManager& getInstance()
	{
		static StateManager single_instance;
		return single_instance;
	}

	void addState(State* state);
	void endState();
	void init();
	void changeState(State* state, bool replace);


	void addObserver(Observer& observer)override;
	void removeObserver(Observer& observer) override;
	void notifyObservers() override;

public:
	std::stack<State*> states;
	StateData stateData;

private:
	StateManager();
	StateManager(const StateManager& root) = delete;
	StateManager& operator=(const StateManager&) = delete;

	std::list<Observer*> observers;
};
