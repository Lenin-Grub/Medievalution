#pragma once
#include "State.h"
#include "GraphicSettings.h"
#include "Button.h"
#include "GameState.h"
#include "SettingsState.h"
// îò
class MainMenuState :
	public State
{
private:
	sf::Text version_text;												// текст версии
	sf::Text mouseCordsText;											// координаты мыши
	std::stringstream cords;											// координаты мыши вывод

	std::map<std::string, Button*> buttons;								// кнопки

	void initView();													// инициализация камеры вида
	void initVariables();												//инициализация чего-либо

	void initGUI();														// инизициализация кнопок
	void updateGUI();													// обновление кнопок
	void renderGUI(sf::RenderTarget& target);							// рендер кнопок

public:
	MainMenuState(StateData* state_data);
	virtual ~MainMenuState();

	void updateEvents();
	void update(const float& dtime);									// обновление
	void render(sf::RenderTarget* target = NULL);						// рендер
};