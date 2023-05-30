#pragma once
//#include "../State/State.h"
//#include "../Settings/WindowSettings.h"
//#include "../State/GameState.h"
//#include "../State/SettingsState.h"
#include "StateManager.h"


//------------------------------------------------------------------------------------------------------------------------
// стейт главного меню игры
class MainMenuState :
	public State
{
private:
	sf::Text mouseCordsText;											// координаты мыши
	std::stringstream cords;											// координаты мыши вывод

//------------------------------------------------------------------------------------------------------------------------
public:
	MainMenuState(StateData* state_data);
	virtual ~MainMenuState();

	void updateEvents();												// обновление событий
	void updateImGui();													// обновление IMGUI
	void update(const float& dtime);									// обновление
	void render(sf::RenderTarget* target = NULL);						// рендер
};
//------------------------------------------------------------------------------------------------------------------------