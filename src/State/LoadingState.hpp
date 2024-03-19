#pragma once
#include "State.hpp"

class StateMachine;

class LoadingState final : public State
{
public:
	LoadingState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void pause()									override;
	void resume()									override;

	void updateEvents()								override;
	void updateImGui()								override;
	void update(const float& dtime)					override;
	void draw(sf::RenderTarget* target = nullptr)	override;

private:

};
