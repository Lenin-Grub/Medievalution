#include "stdafx.h"
#include "LoadingScreanState.h"

void LoadingScreanState::initView()
{
}

void LoadingScreanState::initVariables()
{
}

void LoadingScreanState::initGUI()
{
}

void LoadingScreanState::updateGUI()
{
}

void LoadingScreanState::renderGUI(sf::RenderTarget& target)
{
}


LoadingScreanState::LoadingScreanState(StateData* state_data) :State(state_data)
{
}

LoadingScreanState::~LoadingScreanState()
{
}

void LoadingScreanState::updateEvents()
{
}

void LoadingScreanState::update(const float& dtime)
{
}

void LoadingScreanState::render(sf::RenderTarget* target)
{
		if (!target)
			target = this->window.get();
}
