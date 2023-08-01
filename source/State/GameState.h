#pragma once
#include "../Observer/Observable.hpp"
#include "../World/WorldMap.h"
#include "../Camera/Camera.h"
#include "StateManager.h"

class GameState :
	public State
{
private:
	sf::Text province_name;												
	sf::Text mouseText;;												
	sf::Shader shader;													

	std::stringstream cords;											
	std::list<Observer*> observer_list;

	WorldMap _map;														
	Camera camera;														

private:
	void initEntities();												
	void renderGUI(sf::RenderTarget& target);							

public:
	GameState(StateData* state_data);
	virtual ~GameState();

	void updateEvents() override;												
	void updateImGui() override;												
	void update(const float& dtime) override;									
	void render(sf::RenderTarget* target = nullptr) override;
};