﻿#pragma once
#include "../Animation/Animation.hpp"
#include "../Observer/Observable.hpp"
#include "../World/WorldMap.h"
#include "../Camera/Camera.h"
#include "StateManager.h"

class GameState :
	public State
{
public:
	GameState(StateData* state_data);
	virtual ~GameState();

	void updateEvents() override;
	void updateImGui() override;
	void update(const float& dtime) override;
	void render(sf::RenderTarget* target = nullptr) override;

	void updateObserver() override;

private:
	sf::Text province_name;
	sf::Sprite sprite;
	sf::Texture texture;

	std::stringstream cords;
	std::list<Observer*> observer_list;

	WorldMap world_map;
	Camera camera;
	std::unique_ptr	<Animator> animator;
};