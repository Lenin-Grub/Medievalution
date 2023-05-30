#pragma once
#include "State.h"
#include "MainMenuState.h"
#include "GameState.h"

//------------------------------------------------------------------------------------------------------------------------
// стейт Настроек игры
class SettingsState :
	public State
{
private:
	sf::Text mouseCordsText;											// координаты мыши
	std::stringstream cords;											// координаты мыши вывод
	int item_current = 0;
	//int music_volume = 100;
	//int sound_volume = 100;
	bool play_music = true;
	bool play_sound = true;
//------------------------------------------------------------------------------------------------------------------------
public:
	SettingsState(StateData* state_data);
	virtual ~SettingsState();

	void updateEvents();												// обновление событий
	void updateImGui();													// обновление IMGUI
	void update(const float& dtime);									// обновление
	void render(sf::RenderTarget* target = NULL);						// рендер
};
//------------------------------------------------------------------------------------------------------------------------