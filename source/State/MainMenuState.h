#pragma once
#include "StateManager.h"


//------------------------------------------------------------------------------------------------------------------------
// стейт главного меню игры
class MainMenuState :
	public State
{
private:
	sf::Text mouseCordsText;											// координаты мыши
	std::stringstream cords;											// координаты мыши вывод
	sf::Texture language_rus,language_eng,background;
	sf::Sprite sprite;
	sf::RectangleShape sh;
	std::ifstream i;
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