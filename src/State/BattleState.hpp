#pragma once
#include "../State/State.h"
#include "../Observer/Observable.hpp"

class BattleState :
	public State
{
public:
	BattleState(StateData* state_data);
	virtual ~BattleState();

	void updateEvents() override;
	void updateImGui() override;
	void update(const float& dtime) override;
	void render(sf::RenderTarget* target = nullptr) override;

	void updateObserver() override;

private:
	int i = 0;

};