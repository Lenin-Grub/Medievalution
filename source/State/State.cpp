#include "../stdafx.h"
#include "../State/State.h"

State::State(StateData* state_data):
	state_data(state_data),
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
	core::mouse_pos_screen = sf::Mouse::getPosition();
	core::mouse_pos_window = sf::Mouse::getPosition(*this->window);
	core::mouse_pos_view = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}