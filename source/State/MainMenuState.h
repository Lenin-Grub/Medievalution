#pragma once
#include "../Observer/Observable.hpp"
#include "StateManager.h"


class MainMenuState :
	public State
{
public:
	MainMenuState(StateData* state_data);
	void loadEntities();
	virtual ~MainMenuState();

	void updateEvents();												
	void updateImGui();													
	void update(const float& dtime);									
	void render(sf::RenderTarget* target = nullptr);		

	void updateObserver() override;

private:
	sf::Text mouseCordsText;
	std::stringstream cords;
	sf::Texture language_rus, language_eng, background;
	sf::Sprite sprite;
	sf::RectangleShape shape;
	std::ifstream i;
	std::list<Observer*> observer_list;
};