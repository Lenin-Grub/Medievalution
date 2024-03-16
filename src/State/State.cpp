#include "../stdafx.h"
#include "../State/State.h"

State::State(StateData& state_data):
	state_data(&state_data),
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