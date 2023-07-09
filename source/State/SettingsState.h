#pragma once
#include "StateManager.h"

//------------------------------------------------------------------------------------------------------------------------
// стейт Настроек игры
class SettingsState :
	public State
{
private:
	int item_current = 0;
	bool play_music = true;
	bool play_sound = true;
	sf::Texture background;
	sf::RectangleShape sh;
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