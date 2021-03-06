﻿#pragma once
#include "State.h"
#include "Button.h"
#include "Map.h"
// îò


class GameState :
    public State
{
private:
	sf::Text mouseText;;												// координаты мыши
	std::stringstream cords;											// координаты мыши вывод

	int type = 1;

	bool isBuild = false;												// разрешено ли строительство
	bool isBuldoze = false;												// разрешено ил разрушение

	std::map<std::string, Button*> buttons;								// кнопки
	std::shared_ptr	<Map> map;											// карта

	void initView();													// инициализация камеры вида
	void initVariables();												// инициализация чего-либо
	void initEntities();												// инициализация сущностей

	void initGUI();														// инизициализация кнопок
	void updateGUI();													// обновление кнопок
	void renderGUI(sf::RenderTarget& target);							// рендер кнопок

	void updateView(const float& dtime);								// обновление камеры вида

public:
	GameState(StateData* state_data);
	virtual ~GameState();

	void updateEvents();
	void update(const float& dtime);									// обновление
	void render(sf::RenderTarget* target = NULL);						// рендер
};