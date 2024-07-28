#pragma once
#include "State.hpp"
#include "SettingsState.hpp"
#include "../World/WorldMap.h"
#include "../Camera/Camera.h"

class StateMachine;

class GameState final 
	: public State
{
public:
	GameState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void onDeactivate()								override;
	void onActivate  ()								override;
	void updateEvents()								override;
	void updateImGui ()								override;
	void update(const float& dtime)					override;
	void draw(sf::RenderTarget* target = nullptr)	override;
	bool isLoad      ();

private:
	sf::Text          province_name;
	sf::Sprite        sprite;
	sf::Texture       texture;
	std::stringstream cords;
	WorldMap          world_map;
	Camera            camera;
	bool              is_loaded;
};
