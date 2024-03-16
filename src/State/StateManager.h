#pragma once
#include "State.h"

enum States
{
	State_Intro,
	State_Loading,
	State_Main_Menu,
	State_Settings,
	State_Battle,
	State_Game
};

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	int  add			(std::shared_ptr<State> state);
	void end			();
	void switchTo		(int id, bool replace = false);

	void processEvent	();
	void updateImGui	();
	void update			(float deltaTime);
	void draw			(sf::RenderTarget& window);

public:
	std::unordered_map<int, std::shared_ptr<State>> states;
	std::shared_ptr<State>				current_state;
	StateData							state_data;
	int inserted_state_ID;
};
