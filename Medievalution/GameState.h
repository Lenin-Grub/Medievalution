#pragma once
#include "State.h"
#include "Button.h"
#include "Map.h"


class GameState :
    public State
{
private:
	sf::Text version_text;												// текст версии
	sf::Text mouseCordsText;											// координаты мыши
	std::stringstream cords;											// координаты мыши вывод

	bool isBuild = false;												// разрешено ли строительство
	bool isBuldoze = false;												// разрешено ил разрушение

	std::map<std::string, Button*> buttons;								// кнопки
	std::shared_ptr	<Map> map;											// карта

	void initView();													// инициализация камеры вида
	void initVariables();												//инициализация чего-либо

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