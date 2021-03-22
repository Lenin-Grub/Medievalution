#pragma once
#include "State.h"
#include "Button.h"
#include "CheckBox.h"

class SettingsState :
    public State
{
private:
	sf::Text version_text;												// текст версии
	sf::Text mouseCordsText;											// координаты мыши
	std::stringstream cords;											// координаты мыши вывод
	

	std::map <std::string, Button*> buttons;							// кнопки
	std::map <std::string, CheckBox*> checkboxs;

	void initView();													// инициализация камеры вида
	void initVariables();												//инициализация чего-либо

	void initGUI();														// инизициализация кнопок
	void updateGUI();													// обновление кнопок
	void renderGUI(sf::RenderTarget& target);							// рендер кнопок

public:
	SettingsState(StateData* state_data);
	virtual ~SettingsState();

	void updateEvents();
	void update(const float& dtime);									// обновление
	void render(sf::RenderTarget* target = NULL);						// рендер
};

