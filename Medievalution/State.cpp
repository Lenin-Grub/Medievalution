#include "stdafx.h"
#include "State.h"

State::State(StateData* state_data)
{
	this->stateData = state_data;						//файл отвечающий за все подключения в стейт
	this->window = state_data->window;					//окно
	this->states = state_data->states;					//состояния игры
	this->quit = false;									//выход их игры
	this->fps_counter = state_data->fps_counter;		//счетчик fps
	this->sec_timer = state_data->sec_timer;			//таймер для просчета fps
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
	S::mousePosScreen = sf::Mouse::getPosition();																		// глобальное положение мыши на мониторе
	S::mousePosWindow = sf::Mouse::getPosition(*this->window);															// положение мыши относительно окна приложения
	S::mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));							// положение мыши относительно камеры вида
	S::mousePosGrid = sf::Vector2u(static_cast <unsigned>(S::mousePosView.x) / static_cast<unsigned>(S::gridSize),		// полоэение мыши относительно сетки карты
								static_cast <unsigned>(S::mousePosView.y) / static_cast<unsigned>(S::gridSize));
	this->window->setView(this->window->getDefaultView());																// устанавливаем дефолтную камеру

	if (sec_timer < 1000.f) {																							//fps
		sec_timer += S::dtime;
		fps_counter++;
	}
	else {
		fpsText.setString("FPS: " + to_string(fps_counter));
		sec_timer = 0;
		fps_counter = 0;
	}
}