#pragma once
#include "State.hpp"
#include "../Camera/Camera.h"
#include "../Pathfinding/Pathfinding.h"
#include "../World/Battleground.h"

class StateMachine;

class BattleState final 
	: public State
{
public:
	BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void onDeactivate()									override;
	void onActivate  ()									override;
	void updateEvents()								    override;
	void updateImGui ()								    override;
	void update(const float& dtime)					    override;
	void draw(sf::RenderTarget* target = nullptr)	    override;

private:
	Battleground battleground;
	Pathfinding pathfinding;
	Camera camera;
	int id = 0;
};