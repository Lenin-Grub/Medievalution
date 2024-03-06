#pragma once
#include "../Observer/Observable.hpp"
#include "StateManager.h"

class SettingsState :
	public State
{
public:
	SettingsState(StateData* state_data);
	virtual ~SettingsState();

	void updateEvents();						
	void updateImGui();							
	void update(const float& dtime);			
	void render(sf::RenderTarget* target = nullptr);

	void updateObserver() override;

private:
	void setBackground();

private:
	bool play_music = true;
	bool play_sound = true;
	sf::Texture background;
	sf::RectangleShape shape;

	std::vector<sf::VideoMode> video_modes;
	int resolution_current_id;
	std::string string_resolution;


	std::list<Observer*> observer_list;
};