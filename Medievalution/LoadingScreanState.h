#pragma once
#include "State.h"
#include "GraphicSettings.h"

class LoadingScreanState:
	public State
{
	
	std::stringstream cords;											// координаты мыши вывод


	void initView();													// инициализация камеры вида
	void initVariables();												//инициализация чего-либо

	void initGUI();														// инизициализация кнопок
	void updateGUI();													// обновление кнопок
	void renderGUI(sf::RenderTarget& target);							// рендер кнопок

public:
	LoadingScreanState(StateData* state_data);
	virtual ~LoadingScreanState();

	void updateEvents();
	void update(const float& dtime);									// обновление
	void render(sf::RenderTarget* target);								// рендер
};

