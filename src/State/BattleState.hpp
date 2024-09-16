#pragma once
#include "State.hpp"
#include "../Camera/Camera.h"
#include "../Pathfinding/Pathfinding.h"
#include "../World/Battleground.h"
#include "../World/Editor/Board.hpp"
#include "../World/Editor/SpriteSheet.hpp"


class StateMachine;

class BattleState final 
	: public State
{
public:
	BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace = true);

	void onDeactivate()									override;
	void onActivate  ()									override;
	void updateEvents()								    override;
	void updateImGui ()								    override;
	void update(const float& dtime)					    override;
	void draw(sf::RenderTarget* target = nullptr)	    override;

private:
	Battleground battleground;
	Pathfinding pathfinding;
	Camera camera;
	int id = 0;

	SpriteSheet m_sprite_sheet;
	Board m_board{ 32, 55, 32 };

	// Gui fields to track error messages and application state
	bool m_imported_sheet = false;
	bool m_size_error_msg = false;
	bool m_file_error_msg = false;
	int m_selected_tile_id = 0;
};