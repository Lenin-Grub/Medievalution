#include "../stdafx.h"
#include "../State/State.h"

State::State(StateData* state_data):
	stateData(state_data),
	window(state_data->window),
	states(state_data->states),
	quit(false),
	replace(false)
{
}

State::~State()
{
}

const bool& State::getQuit() const
{
	return this->quit;
}

void State::endState()
{
	this->quit = true;
}

void State::updateMousePositions(sf::View* view)
{
	core::mousePosScreen = sf::Mouse::getPosition();																		
	core::mousePosWindow = sf::Mouse::getPosition(*this->window);															
	core::mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));							
}