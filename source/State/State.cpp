#include "../stdafx.h"
#include "../State/State.h"

//------------------------------------------------------------------------------------------------------------------------
// конструктор класса State
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

//------------------------------------------------------------------------------------------------------------------------
// проверка вышли ли из программы
const bool& State::getQuit() const
{
	return this->quit;
}

//------------------------------------------------------------------------------------------------------------------------
// закрытие текущего State
void State::endState()
{
	this->quit = true;
}

//------------------------------------------------------------------------------------------------------------------------
// обновление позиции мыши
void State::updateMousePositions(sf::View* view)
{
	core::mousePosScreen = sf::Mouse::getPosition();																		
	core::mousePosWindow = sf::Mouse::getPosition(*this->window);															
	core::mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));							
}