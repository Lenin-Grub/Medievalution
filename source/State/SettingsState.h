#pragma once
#include "../Observer/Observable.hpp"
#include "StateManager.h"

class SettingsState :
	public State
{
private:
	int item_current = 0;
	bool play_music = true;
	bool play_sound = true;
	sf::Texture background;
	sf::RectangleShape sh;

	std::list<Observer*> observer_list;
	std::vector<sf::VideoMode> video_modes;

	char resolution[];

public:
	SettingsState(StateData* state_data);
	virtual ~SettingsState();

	void updateEvents();						
	void updateImGui();							
	void update(const float& dtime);			
	void render(sf::RenderTarget* target = NULL);
};
