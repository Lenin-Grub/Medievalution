#pragma once
#include <UI/IDisplay.hpp>
#include <SFML/Graphics.hpp>
#include <World/BattleMap/BattleMap.hpp>

class MenuDisplay
	: public UI::IDisplay
{
public:
	MenuDisplay(sf::RenderWindow& window, BattleMap& battle_map);
	virtual ~MenuDisplay() = default;

	void draw() override;
	void update(const float& delta_time) override;

public:
	bool show_demo_window       = false;
	bool show_another_window    = false;
	bool show_animation_window  = false;
	bool show_map_editor_window = false;
	bool show_metrics           = false;

private:
	BattleMap& battle_map;
	sf::RenderWindow& window;
};