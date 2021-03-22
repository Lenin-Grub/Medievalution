#include "stdafx.h"
#include "State.h"

State::State(StateData* state_data)
{
	this->stateData = state_data;
	this->window = state_data->window;
	this->states = state_data->states;
	this->quit = false;
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
	S::mousePosScreen = sf::Mouse::getPosition();																		
	S::mousePosWindow = sf::Mouse::getPosition(*this->window);															
	S::mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));							
	S::mousePosGrid = sf::Vector2u(	static_cast <unsigned>(S::mousePosView.x) / static_cast<unsigned>(S::gridSize),		
									static_cast <unsigned>(S::mousePosView.y) / static_cast<unsigned>(S::gridSize));															
}

void State::updateFPS()
{
	if (sec_timer < 1000.f) {
		sec_timer += S::dtime;
		fps_counter++;
	}
	else 
	{
		fpsText.setString("FPS: " + to_string(fps_counter));
		sec_timer = 0;
		fps_counter = 0;
	}
}
