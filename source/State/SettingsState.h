#pragma once
#include "../Observer/Observable.hpp"
#include "StateManager.h"

//------------------------------------------------------------------------------------------------------------------------
// стейт Настроек игры
class SettingsState :
	public State,
	public Observable
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

//------------------------------------------------------------------------------------------------------------------------
private:
//------------------------------------------------------------------------------------------------------------------------
public:
	SettingsState(StateData* state_data);
	virtual ~SettingsState();

	void updateEvents();												// обновление событий
	void updateImGui();													// обновление IMGUI
	void update(const float& dtime);									// обновление
	void render(sf::RenderTarget* target = NULL);						// рендер

	void addObserver(Observer& observer)	override;
	void removeObserver(Observer& observer)	override;
	void notifyObservers()					override;
};
//------------------------------------------------------------------------------------------------------------------------