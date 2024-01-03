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
	sf::RectangleShape shape;

	std::list<Observer*> observer_list;
	std::vector<sf::VideoMode> video_modes;

	int item_current_idx = 0;

public:
	SettingsState(StateData* state_data);
	virtual ~SettingsState();

	void updateEvents();						
	void updateImGui();							
	void update(const float& dtime);			
	void render(sf::RenderTarget* target = nullptr);

	void updateObserver() override;
};